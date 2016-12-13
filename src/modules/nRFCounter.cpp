
#include <cassert>
#include <inttypes.h>
#include <modules/nRFCounter.h>
#include "../drivers/lowFrequencyClock.h"
#include "../drivers/counter.h"
#include "../drivers/compareRegister.h"

/*
 * Private data.
 * If you use the underlying peripherals elsewhere, you must coordinate.
 */
namespace {

typedef void (*funcPtr)();

LowFrequencyClock lowFrequencyClock;
Counter counter;



// MSB of LongClock incremented by overflow interrupt handler
uint32_t mostSignificantBits;

/*
 * RTC device has three compare registers.
 * This class owns them also, and a callback for each's interrupt.
 * Callback is also used as a flag for 'started'
 */
funcPtr timerCallback[3];
CompareRegister compareRegisters[3];


void startXtalOscillator() {
	lowFrequencyClock.configureXtalSource();
	// assert source is LFXO

	lowFrequencyClock.start();
	// assert LFC is running
}


void initCompareRegs() {
	// This is expanded because the hw constants are defined by unparameterized macros
	// Parameters of compareRegisters are fixed by hw design
	compareRegisters[0].init(
			NRF_RTC_EVENT_COMPARE_0,
			RTC_EVTEN_COMPARE0_Msk,
			NRF_RTC_INT_COMPARE0_MASK,
			0	// index
	);
	compareRegisters[1].init(
			NRF_RTC_EVENT_COMPARE_1,
			RTC_EVTEN_COMPARE1_Msk,
			NRF_RTC_INT_COMPARE1_MASK,
			1	// index
	);
	compareRegisters[2].init(
			NRF_RTC_EVENT_COMPARE_2,
			RTC_EVTEN_COMPARE2_Msk,
			NRF_RTC_INT_COMPARE2_MASK,
			2	// index
	);

	timerCallback[0] = nullptr;
	timerCallback[1] = nullptr;
	timerCallback[2] = nullptr;

}

} // namespace




/*
 * One handler for many interrupt sources (overflow, and compare regs)
 * Many sources can be pending, so handle them all.
 *
 * Overrides weak default handler defined by gcc_startup_nrf52.c.
 */
extern "C" {	// Binding must be "C" to override default handler.
void RTC0_IRQHandler(void)
{
	// Source event is overflow
	if ( counter.isOverflowEvent() ) {
		mostSignificantBits++;
		counter.clearOverflowEvent();
		// assert interrupt still enabled
	}

	// Loop over compare regs
	for (int i=0; i<LongClockTimer::CountTimerInstances; i++ ) {
		if ( compareRegisters[i].isEvent() ) {
			compareRegisters[i].disableInterrupt();
			timerCallback[i]();	// call callback
			timerCallback[i] = nullptr;	// show not running
			compareRegisters[i].clearEvent();

			// One-shot: interrupt is disabled
			// Event is clear but can be set again on rollover
		}
	}

	/*
	 * If any events have triggered after we checked them,
	 * they will still trigger an interrupt and this handler will be called again.
	 */
}
}	// extern "C"


void LongClockTimer::init() {

	mostSignificantBits = 1;

	// RTC requires LFC started
	startXtalOscillator();

	// Docs don't say this can't be done while counter is running
	counter.configureOverflowInterrupt();
	// assert interrupt is enabled and mostSignificantBits will increment on overflow

	// assert prescaler is default of 0, i.e. 30uSec tick

	counter.start();

	initCompareRegs();

	// assert counter is running
	// assert interrupt enabled for overflow
	// assert compareRegisters are configured by default to disabled interrupt w/ nullptr callbacks
}

LongTime LongClockTimer::getNowTime() {
	OSTime currentOSClockTicks = counter.ticks();
	// Concatenate MSB and LSB.  Portable?
	LongTime result = mostSignificantBits;
	result = result << OSClockCountBits;	// Left shift result, fill LSB with zero
	result = result | currentOSClockTicks;	// Bit-wise OR into LSB.  Addition would work also.
	return result;
}



void LongClockTimer::startTimer(
		unsigned int index,
		unsigned int timeout,
		void (*aTimeoutCallback)()){
	assert(timeout < MaxTimeout);
	assert(timeout >= 2);
	assert(index < CountTimerInstances);

	// Not legal to start Timer already started and not timed out or canceled.
	if (isTimerStarted(index)) {
		assert(false);
		return;	// No error result, must be tested with assertions enabled.
	}

	// remember callback
	timerCallback[index] = aTimeoutCallback;

	compareRegisters[index].set(timeout);
	// event not enabled yet
	compareRegisters[index].enableInterrupt();
}

bool LongClockTimer::isTimerStarted(unsigned int index) {
	return ! (timerCallback[index] == nullptr);
}

void LongClockTimer::cancelTimer(unsigned int index){
	/*
	 * Legal to cancel Timer that has not been started.
	 */
	// TODO discuss possible race
	compareRegisters[index].disableInterrupt();
	timerCallback[index] = nullptr;
}




#include "counter.h"

// Use HAL
#include "nrf_rtc.h"

namespace {

/*
 * Interrupt must be enabled in NVIC as well as in device.
 */
void enableNvicIRQ_RTC0() {
	NVIC_ClearPendingIRQ(RTC0_IRQn);
	NVIC_EnableIRQ(RTC0_IRQn);
}

}  // namespace




void Counter::start(){
	// Start (power on)
	nrf_rtc_task_trigger(NRF_RTC0, NRF_RTC_TASK_START);
	/*
	 * Exists a delay until first increment.
	 * Delay due to oscillator startup.
	 * Delay depends on source oscillator.
	 * Delay for LFXO is 0.25 seconds.
	 * Delay for LFRC is 0.6 msec
	 * See CLOCK>LFCLK>LFXO on LFRC electrical specifications
	 *
	 * Here, we don't wait: no guarantee that first tick has come.
	 *
	 * TODO, use TRIGOVRFLW to ensure first tick has come.
	 */
}



void Counter::configureOverflowInterrupt(){

	/*
	 * On a task/event architecture, device generates event which generates interrupt.
	 * Both event and/or interrupt can be enabled.
	 *
	 * !!! Interrupt must be enabled in device AND in nvic
	 */

	// Writes 1 to bit of EVTEN reg.  Does not affect other enabled events.
	nrf_rtc_event_enable(NRF_RTC0, RTC_EVTEN_OVRFLW_Msk);

	// Writes 1 to bit of INTENSET reg.  Does not affect other enabled interrupts.
	nrf_rtc_int_enable(NRF_RTC0, NRF_RTC_INT_OVERFLOW_MASK);

	enableNvicIRQ_RTC0();

	// Interrupt can come at any time.
	// Usually, this is called shortly after starting Counter, so interrupt will come after period of Counter
}


void Counter::clearOverflowEvent(){
	nrf_rtc_event_clear(NRF_RTC0, NRF_RTC_EVENT_OVERFLOW);
}

bool Counter::isOverflowEvent(){
	return nrf_rtc_event_pending(NRF_RTC0, NRF_RTC_EVENT_OVERFLOW);
}

OSTime Counter::ticks(){
	return nrf_rtc_counter_get(NRF_RTC0);
}


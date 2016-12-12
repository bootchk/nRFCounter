
#include <inttypes.h>
// The following include includes sdk_config.h
//#include "nrf_drv_clock.h"
#include "nrf_clock.h"

#include "lowFrequencyClock.h"



// Uses HAL instead of nrf_drv_clock



void LowFrequencyClock::start() {
	/*
	 * Assumes source configured prior, else default source.
	 */
	nrf_clock_event_clear(NRF_CLOCK_EVENT_LFCLKSTARTED);
	nrf_clock_task_trigger(NRF_CLOCK_TASK_LFCLKSTART);

	while ( ! isStarted() ) {}	// spin

	// We leave the started event set to indicate state for isStarted()

	// assert isStarted()
}


bool LowFrequencyClock::isStarted() {
	return nrf_clock_event_check(NRF_CLOCK_EVENT_LFCLKSTARTED);
}

/*
 * Source cannot be configured while running (Nordic requires.)
 * If you don't call this, source is reset default of LFRC.
 */
void LowFrequencyClock::configureXtalSource() {
	nrf_clock_lf_src_set(NRF_CLOCK_LFCLK_Xtal);
}


#include <inttypes.h>
#include "sdk_config.h"
#include "nrf_drv_clock.h"

#include "lowFrequencyClock.h"

void init() {
	uint32_t err = nrf_drv_clock_init();
	APP_ERROR_CHECK(err);
	nrf_drv_clock_lfclk_request(NULL);
}

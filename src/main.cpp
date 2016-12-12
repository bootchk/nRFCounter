
/*
 * Test main for nRFCounter library.
 *
 * Minimal: only tests the LongClock.
 * This project is configured to build a library, and not an executable: this main is an example, non-functional.
 *
 * For more tests, see testCounter project.
 * That project is configured to build for a target nRF5x chip.
 *
 * Copyright 2016 lloyd konneker
 */



#include "modules/longClockTimer.h"


#include "SEGGER_RTT.h"


void logLongLong(uint64_t value ){
	// Print on one line
	(void) SEGGER_RTT_printf(0, "x%04x", *(((uint32_t*) &value) + 1)  );	// MS word
	(void) SEGGER_RTT_printf(0, "%04x\n", value);	// LS word and newline

}

LongClockTimer longClockTimer;

int main() {
	// assert embedded system startup is done and calls main.

	longClockTimer.init();

	while (true) {
		// delay
		logLongLong( longClockTimer.getNowTime() );
		// longClockTimer.timer1.start(100);
	}

	return 0;
}

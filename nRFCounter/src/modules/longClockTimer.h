
#include "timer.h"
#include "lowFrequencyClock.h"

/*
 * Thin wrapper around RTC in Nordic nRF52/51
 */
class LongClockTimer {
public:
	/*
	 * Each timer uses a separate compare register.
	 */
	static LowFrequencyClock lowFrequencyClock;

	static Timer timer1;
	static Timer timer2;

	static void init();

	//static void startTimer1(int);
};

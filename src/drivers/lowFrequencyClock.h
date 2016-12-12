
/*
 * Low frequence oscillator (not really a clock.)
 *
 * Valid call sequences:
 *
 * configureXtalSource, start, isStarted (normal call sequence)
 * start, isStarted (uses default LFRC oscillator source)
 * isStarted, start, isStarted (first call returns false)
 *
 */
class LowFrequencyClock {
public:
	static void configureXtalSource();

	static void start();

	/*
	 * is event signifying start?
	 * This does not guarantee clock is still running.
	 * There is no code here to stop the clock.
	 * We assume the PowerManager functions as designed and does not power down clock.
	 */
	static bool isStarted();
};

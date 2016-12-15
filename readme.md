
A thin wrapper around the RTC peripheral of the Nordic nRF5x family of embedded radio SoC chips.

Object oriented (C++)

The same but improved code is also contained in nRF5x library project.

Provides
-
LongClock 56-bit clock with resolution of 30uSec.  

Timers, one-shot, 24-bit with max timeout of ~500 seconds.  Unlike Nordic's app_timer, there are few timers, one per compare register of the device.

Is not a Real Time Clock in the sense that it provides seconds and calendar.

The LongClock cannot be stopped (the mcu can be paused, but the RTC device always runs.  Thus the lowest power mode requires < 2uA of the RTC, plus whatever the stopped mcu requires for RAM retention.)

Motivation
-
    - Nordic's app_timer library is of unknown quality
    - Nordic's drivers are general purpose, not well explained
    - both are overweight for many use cases.

Simplifications
-
    - LongClock always runs and can't be stopped
    - the used peripheral is hard coded (RTC0, could use RTC1, etc.)
    - each Timer uses one of the compare registers of the RTC0 peripheral
    - little run-time error checking, instead assertions are used
    - compatible with SoftDevice (which uses different RTC1)
    
Implementation Notes
-
Uses HAL, not nrf_drv: probably less robust, but easier to understand.  In other words, bare metal, direct access to device registers.

Uses the 32-kHz xtal clock source, so is as accurate as most quartz hand watches.

The "RTC" acronym for the peripheral is misguided.  It is a counter, but "RTC" usually means "real time clock" that keeps accurate wall time, providing seconds and calendar. Here the library provides something closer to a real time clock, and timers.


Building
-

The artifacts are libraries cross-compiled for targets nRF51 and nRF52 (ARM M0 and M4.)

This is an eclipse project, using Eclipse's build system

The project in this repository is an Eclipse project.
The project's Eclipse build configuration contains many include paths and other build settings.
Such settings are not defined in a Makefile.
You will need to modify the paths in the project's Properties>C/C++Build>Settings>CrossARMC++>Includes.

See Also
-
A companion project is testnRFCounter.

A using project will be nRFrawProtocol and sleepSyncAgent.

TODO
-
Make the project less specific to my dev system.

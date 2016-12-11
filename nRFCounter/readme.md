
A thin wrapper around the RTC peripheral of the Nordic nRF5x family of embedded radio SoC chips.

Object oriented.

Motivation: Nordic's app_timer library seems buggy and is overweight for many use cases.

The "RTC" acronym for the peripheral is misguided.  It is a counter, but "RTC" usually means "real time clock".  
Here I will use *RTCounter* to mean the peripheral.

The project in this repository is an Eclipse project.
The project's Eclipse build configuration contains many include paths and other build settings.
Such settings are not defined in a Makefile.
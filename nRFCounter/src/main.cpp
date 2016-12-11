
/*
 * Test main for LongClockTimer library.
 *
 * Copyright 2016 lloyd konneker
 */



#include "modules/longClockTimer.h"

#ifdef TODO
// callbacks


void onWorkMsg(WorkPayload work) {
	// SleepSyncAgent received and queued a work msg.
	// FUTURE schedule low priority work thread/task to do work
	// realtime constrained
	(void) work;
}

void onSyncPoint() {
	// Called at every SyncPoint.
	// FUTURE do something in sync with other units.
}
#endif

LongClockTimer longClockTimer;

int main() {
	// assert embedded system startup is done and calls main.

	longClockTimer.init();

	while (true) {
		longClockTimer.timer1.start(100);
	}

	return 0;
}

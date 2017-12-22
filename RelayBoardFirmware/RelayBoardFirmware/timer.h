/*
 * timer.h
 *
 * Created: 2015-01-18 12:03:09
 *  Author: Marcin Bajer
 */ 

#include "global.h"
#ifndef TIMER_H_
#define TIMER_H_

#define TIMER_MS_TO_TIMER(x) x/100

void timerInit();
extern bool timerTriggered;

#endif /* TIMER_H_ */
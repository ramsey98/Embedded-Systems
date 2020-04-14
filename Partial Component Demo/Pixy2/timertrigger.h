/*
 * timertrigger.h
 *
 *  Created on: Apr 14, 2020
 *      Author: Matthew
 */

#ifndef TIMERTRIGGER_H_
#define TIMERTRIGGER_H_

#include <ti/drivers/Timer.h>
#include "debug.h"

#define TIMER_TRIGGER_PERIOD (10)

void timerTriggerCallback(Timer_Handle myHandle);
void timerTriggerInit();
void timerTriggerStart();

#endif /* TIMERTRIGGER_H_ */

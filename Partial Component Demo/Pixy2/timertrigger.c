/*
 * timertrigger.c
 *
 *  Created on: Apr 14, 2020
 *      Author: Matthew
 */

#include "timertrigger.h"

Timer_Handle timertrig;

void timerTriggerCallback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTER_ISR_TIMER1);
    GPIO_write(CONFIG_GPIO_8, CONFIG_GPIO_LED_OFF);
    dbgOutputLoc(LEAVE_ISR_TIMER1);
}

void timerTriggerInit()
{
    Timer_Params timer_params;
    Timer_Params_init(&timer_params);
    timer_params.period = TIMER_TRIGGER_PERIOD;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_ONESHOT_CALLBACK;
    timer_params.timerCallback = timerTriggerCallback;
    timertrig = Timer_open(CONFIG_TIMER_2, &timer_params);
    if (timertrig == 0) ERROR;
}

void timerTriggerStart() {
    if (Timer_start(timertrig) == Timer_STATUS_ERROR) ERROR;
}


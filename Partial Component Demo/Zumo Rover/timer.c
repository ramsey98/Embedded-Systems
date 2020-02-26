/*
 * timer.c
 *
 *  Created on: Feb 25, 2020
 *      Author: Holden Ramsey
 */
#include "timer.h"

void timerInit()
{
    Timer_Handle timer;
    Timer_Params timer_params;
    Timer_Params_init(&timer_params);
    timer_params.period = TIMER_PERIOD;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timerCallback;

    timer = Timer_open(CONFIG_TIMER_0, &timer_params);
    if (timer == NULL)
    {
        ERROR;
    }

    if (Timer_start(timer) == Timer_STATUS_ERROR)
    {
        ERROR;
    }
}

void timerCallback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTER_ISR_TIMER2);
    sendMsgToPIDQ(TIMER, 0);
    clearCounts();
    pollSensor();
    //pollPixy();
    dbgOutputLoc(LEAVE_ISR_TIMER2);
}



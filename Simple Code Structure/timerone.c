/*
 * timerone.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */


#include "timerone.h"
uint32_t count;
Timer_Handle timer0;
Timer_Params timer_params;

void timerSecondCallback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTER_ISR);
    count = Timer_getCount(myHandle) - count;
    unsigned int time = count/1000;
    sendTimeMsgToQ1(time);
    dbgOutputLoc(LEAVE_ISR);
}

void timerOneInit()
{
    Timer_Params_init(&timer_params);
    timer_params.period = 1000000;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timerSecondCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &timer_params);
    if (timer0 == 0)
    {
        halt();
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR)
    {
        halt();
    }
    count = Timer_getCount(timer0);

}

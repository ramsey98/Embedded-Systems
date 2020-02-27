/*
 * timerone.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */


#include "timerone.h"

void timerSecondCallback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTER_ISR_TIMER1);
    uint32_t time = TIMER0_PERIOD/1000;

    int ret1 = sendTimeMsgToQ1(time);
    int ret2 = sendTimeMsgToPixyQ1(time);

    if(ret1 < 0 || ret2 < 0) {
        halt();
    }

    dbgOutputLoc(LEAVE_ISR_TIMER1);
}

void timerOneInit()
{
    Timer_Handle timer0;
    Timer_Params timer_params;
    Timer_Params_init(&timer_params);
    timer_params.period = TIMER0_PERIOD;
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

}

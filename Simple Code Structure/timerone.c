/*
 * timerone.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */


#include "timerone.h"

void timerSecondCallback(Timer_Handle myHandle)
{
    count -= Timer_getCount(myHandle);
    sendSensorMsgToQ1(count);
}

void timerOneInit()
{
    Timer_init();
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerSecondCallback;

    Timer_Handle timer0 = Timer_open(CONFIG_TIMER_0, &params);
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

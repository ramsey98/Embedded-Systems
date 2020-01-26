/*
 * timerone.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */


#include "timerone.h"

void timerSecondCallback(Timer_Handle myHandle)
{
    sendSensorMsgToQ1(0);//change to time count
}

void timerOneInit()
{
    Timer_init();
    Timer_Params_init(&params);
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerSecondCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == 0)
    {
        halt();
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR)
    {
        halt();
    }

}

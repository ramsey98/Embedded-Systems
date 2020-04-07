/*
 * timer.c
 *
 *  Created on: Mar 16, 2020
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
    if (timer == NULL) ERROR;
    if (Timer_start(timer) == Timer_STATUS_ERROR) ERROR;
}

void timerCallback(Timer_Handle myHandle)
{
    static int count = 0;
    MQTTMsg msg = {0, 0};
    dbgOutputLoc(ENTER_ISR_TIMER);
    if(count % 2 == 0)
    {
        //pollSensor();
    }
    if(count % 10 == 0)
    {
        msg.type = JSON_TYPE_DEBUG;
        msg.value = 0;
        sendMsgToMQTTQFromISR(msg);
    }
    if(count % 20 == 0)
    {
        //sendMsgToPixyQFromISR(PIXY_COLOR);
        sendMsgToPIDQFromISR(TIMER, EMPTY);
        msg.type = JSON_TYPE_STATS;
        sendMsgToMQTTQFromISR(msg);
    }
    if(count == 100)
    {
        count = 0;
    }
    else
    {
        count++;
    }
    dbgOutputLoc(LEAVE_ISR_TIMER);
}

/*
 * timertwo.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#include "timer.h"

void timer100MSCallback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTER_ISR_TIMER2);
    static int count = 0, pixy_polls = 0, sensor_polls = 0;
    uint32_t time = TIMER100_PERIOD/100;

    //Pixy Section
    if(count % 2 == 0) {
        if(sendPollMsgToPixyQ1() < 0) ERROR;
        pixy_polls++;
    }

    //Sensor Section
    sendSensorMsgToQ(SENSOR_TYPE_TRIGGER);
    sensor_polls++;

    MQTTMsg msg = {0, 0};
    if(count == 10)
    {
        //Pixy Section
        if(sendTimeMsgToPixyQ1(time) < 0) ERROR;

        //Sensor Section
        sendSensorMsgToQ(SENSOR_TYPE_SUM);
        msg.type = 1;

        //Poll stats
        MQTTMsg msgPoll = {6, pixy_polls, sensor_polls};
        sendMsgToMQTTQFromISR(msgPoll);
        sendMsgToMQTTQFromISR(msg);
        count = sensor_polls = pixy_polls = 0;
    }
    else if(count % 5 == 0)
    {
        msg.type = 2;
        sendMsgToMQTTQFromISR(msg);
    }
    count++;

    dbgOutputLoc(LEAVE_ISR_TIMER2);
}

void timer100MSInit()
{
    Timer_Handle timer1;
    Timer_Params timer_params;
    Timer_Params_init(&timer_params);
    timer_params.period = TIMER100_PERIOD;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timer100MSCallback;

    timer1 = Timer_open(CONFIG_TIMER_0, &timer_params);
    if (timer1 == NULL)
    {
        ERROR;
    }

    if (Timer_start(timer1) == Timer_STATUS_ERROR)
    {
        ERROR;
    }

}


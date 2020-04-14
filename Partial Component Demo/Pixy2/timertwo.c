/*
 * timertwo.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#include "timertwo.h"

static ADC_Handle adc;

void timer100MSCallback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTER_ISR_TIMER2);
    static int count = 0;

    //Pixy Section
    if(sendPollMsgToPixyQ1() < 0) ERROR;

    //Sensor Section
    sendSensorMsgToQ(SENSOR_TYPE_TRIGGER);

    //MQTT Section
    MQTTMsg msg = {0, 0};
    if(count == 10)
    {
        msg.type = 1;
        count = 0;
    }
    else
    {
        msg.type = 2;
    }
    sendMsgToMQTTQFromISR(msg);
    count++;

    dbgOutputLoc(LEAVE_ISR_TIMER2);
}

void adcInit()
{
    ADC_init();
    ADC_Params adc_params;
    ADC_Params_init(&adc_params);
    adc = ADC_open(CONFIG_ADC_0, &adc_params);
    if (adc == NULL)
    {
        ERROR;
    }
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

    timer1 = Timer_open(CONFIG_TIMER_1, &timer_params);
    if (timer1 == NULL)
    {
        ERROR;
    }

    if (Timer_start(timer1) == Timer_STATUS_ERROR)
    {
        ERROR;
    }

}


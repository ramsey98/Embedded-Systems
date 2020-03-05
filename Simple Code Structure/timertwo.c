/*
 * timertwo.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#include "timertwo.h"

static ADC_Handle adc;

void timer75Callback(Timer_Handle myHandle)
{
    dbgOutputLoc(ENTER_ISR_TIMER2);
//    int res;
//    res = conversion(adc);
//    sendSensorMsgToQ1(res);
    sendMsgToEncoderQueue();
    dbgOutputLoc(LEAVE_ISR_TIMER2);
}

int conversion(ADC_Handle adc)
{
    // Blocking mode conversion
    uint16_t adcValue;
    uint32_t result;
    int res = ADC_convert(adc, &adcValue);
    if (res == ADC_STATUS_SUCCESS)
    {
        result = ADC_convertRawToMicroVolts(adc, adcValue);
    }
    else
    {
        result = -1; //conversion failed
    }
    return result;
}

void adcInit()
{
    ADC_init();
    ADC_Params adc_params;
    ADC_Params_init(&adc_params);
    adc = ADC_open(CONFIG_ADC_0, &adc_params);
}

Timer_Handle timerTwoInit()
{
    Timer_Handle timer2;
    Timer_Params timer_params;
    Timer_Params_init(&timer_params);
    timer_params.period = 75000;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timer75Callback;

    timer2 = Timer_open(CONFIG_TIMER_1, &timer_params);
    if (timer2 == NULL)
    {
        ERROR;
    }

    return timer2;

//    if (Timer_start(timer1) == Timer_STATUS_ERROR)
//    {
//        ERROR;
//    }

}


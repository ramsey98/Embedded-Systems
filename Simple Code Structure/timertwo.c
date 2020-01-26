/*
 * timertwo.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#include "timertwo.h"

void timer75Callback(Timer_Handle myHandle)
{
    ADC_Handle   adc;
    ADC_Params   params;
    int res;

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_0, &params);
    res = conversion(adc);
    ADC_close(adc);
    sendSensorMsgToQ1(res);
}

int conversion(ADC_Handle adc)
{
    /* Blocking mode conversion */
    uint16_t adcValue;
    uint32_t result;
    res = ADC_convert(adc, &adcValue);
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

void timerTwoInit()
{
    Timer_Handle timer1;
    Timer_Params params;
    Timer_init();
    Timer_Params_init(&params);
    params.period = 75000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timer75Callback;

    timer1 = Timer_open(CONFIG_TIMER_1, &params);
    if (timer1 == NULL)
    {
        halt();
    }

    if (Timer_start(timer1) == Timer_STATUS_ERROR)
    {
        halt();
    }

}


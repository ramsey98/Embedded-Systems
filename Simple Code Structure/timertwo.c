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
    uint16_t adcValue;
    uint32_t adcValueMicroVolt;
    int_fast16_t res = ADC_convert(adc, &adcValue);
    int result;
    if (res == ADC_STATUS_SUCCESS)
    {
        adcValueMicroVolt = ADC_convertRawToMicroVolts(adc, adcValue);
        result = conversion(adcValueMicroVolt);
    }
    if (result != -1)
    {
        sendSensorMsgToQ1(result);
    }
    dbgOutputLoc(LEAVE_ISR_TIMER2);
}

int conversion(uint32_t sensorVal)
{
    //convert to mm here
    int sensorConv = sensorVal/1000;
    /*
    int result;
    if (sensorConv>1300)
        result = 10;
    else if (sensorConv < 200)
        result = 80;
    */
    return sensorConv;
}

void adcInit()
{
    ADC_init();
    ADC_Params adc_params;
    ADC_Params_init(&adc_params);
    adc = ADC_open(CONFIG_ADC_0, &adc_params);
    if (adc == NULL)
    {
        halt();
    }
}

void timerTwoInit()
{
    Timer_Handle timer1;
    Timer_Params timer_params;
    Timer_Params_init(&timer_params);
    timer_params.period = TIMER1_PERIOD;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timer75Callback;

    timer1 = Timer_open(CONFIG_TIMER_1, &timer_params);
    if (timer1 == NULL)
    {
        halt();
    }

    if (Timer_start(timer1) == Timer_STATUS_ERROR)
    {
        halt();
    }

}


/*
 * sensor.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "sensor.h"

static ADC_Handle adc;

void *sensorThread(void *arg0)
{
    adcInit();
    timerInit();
    dbgOutputLoc(ENTER_TASK);
    SENSOR_DATA curState;
    curState.state = Sensor_Init;
    uint16_t sensorVal = 0;
    int success = sensor_fsm(&curState, sensorVal);
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromSensorQ(&sensorVal);
        success = sensor_fsm(&curState, sensorVal);
        if(success == -1 || received == -1)
        {
            ERROR;
        }
    }
}

void timerCallback(Timer_Handle myHandle)
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
        sendSensorMsgToQ(result);
    }
    else
    {
        ERROR;
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
        ERROR;
    }
}

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
    if (timer == NULL)
    {
        ERROR;
    }

    if (Timer_start(timer) == Timer_STATUS_ERROR)
    {
        ERROR;
    }

}


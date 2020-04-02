/*
 * sensor.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "sensor.h"

static ADC_Handle adc;
static int sensorValuesLookup[DICTLEN] = {0};
static int sensorDistLookup[DICTLEN] = {0};

void *sensorThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    adcInit();
    SENSOR_DATA curState;
    curState.state = Sensor_Init;
    uint16_t sensorVal = 0;
    sensor_fsm(&curState, sensorVal);
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromSensorQ(&sensorVal);
        sensor_fsm(&curState, sensorVal);
    }
}

void adcInit()
{
    ADC_init();
    ADC_Params adc_params;
    ADC_Params_init(&adc_params);
    adc = ADC_open(CONFIG_ADC_0, &adc_params);
    if (adc == NULL) ERROR;
}

void pollSensor()
{
    int result = 0;
    uint16_t adcValue;
    uint32_t adcValueMicroVolt;
    int_fast16_t res = ADC_convert(adc, &adcValue);
    if (res == ADC_STATUS_SUCCESS)
    {
        adcValueMicroVolt = ADC_convertRawToMicroVolts(adc, adcValue);
        result = conversion(adcValueMicroVolt);
        if (result != -1)
        {
            sendSensorMsgToQ(result);
        }
        else ERROR;
    }
}

int conversion(uint32_t sensorVal)
{
    int i, sensorConv = 0;
    for(i = 0; i < DICTLEN; i++)
    {
        if(sensorVal < sensorValuesLookup[i])
        {
            sensorConv = sensorDistLookup[i];
            break;
        }
    }
    return sensorConv;
}


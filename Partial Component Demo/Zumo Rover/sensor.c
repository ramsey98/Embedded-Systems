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
    adcInit();
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

void pollSensor()
{
    int result = 0, sent = 0;
    uint16_t adcValue;
    uint32_t adcValueMicroVolt;
    int_fast16_t res = ADC_convert(adc, &adcValue);
    if (res == ADC_STATUS_SUCCESS)
    {
        adcValueMicroVolt = ADC_convertRawToMicroVolts(adc, adcValue);
        result = conversion(adcValueMicroVolt);
        if (result != -1)
        {
            sent = sendSensorMsgToQ(result);
            if(sent == -1)
            {
                ERROR;
            }
        }
        else
        {
            ERROR;
        }
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


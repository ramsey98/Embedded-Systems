/*
 * sensor.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "sensor.h"

static ADC_Handle adc;
const lookupTable sensorLookup[DICTLEN] = {{840000,12},
                                           {770000,14},
                                           {670000,16},
                                           {600000,18},
                                           {530000,20},
                                           {500000,22},
                                           {460000,24},
                                           {420000,26},
                                           {400000,28},
                                           {370000,30},
                                           {100,0}};

void *sensorThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint16_t sensorVal = 0;
    static int total = 0, count = 0, avg = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromSensorQ(&sensorVal);
        total+=sensorVal;
        count++;
        if(count == 5)
        {
            avg = total/count;
            sendMsgToPIDQ(SENSOR, avg);
        }
    }
}

void adcInit()
{
    ADC_Params adc_params;
    ADC_Params_init(&adc_params);
    adc = ADC_open(CONFIG_ADC_0, &adc_params);
    if (adc == NULL) ERROR;
}

void pollSensor()
{
    uint16_t adcValue;
    if(ADC_convert(adc, &adcValue) != ADC_STATUS_SUCCESS) ERROR;
    uint32_t adcValueMicroVolt = ADC_convertRawToMicroVolts(adc, adcValue);
    int result = conversion(adcValueMicroVolt);
    sendSensorMsgToQ(result);
}

int conversion(uint32_t sensorVal)
{
    int i, sensorConv = 0;
    for(i = 0; i < DICTLEN; i++)
    {
        if(sensorVal > sensorLookup[i].val)
        {
            sensorConv = sensorLookup[i].dist;
            break;
        }
    }
    if(sensorConv == 0) ERROR;
    return sensorConv;
}


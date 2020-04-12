/*
 * sensor.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "sensor.h"
#include "mqtt_queue.h"
#include "json_parse.h"

static ADC_Handle adc;
const lookupTable sensorLookup[DICTLEN] = {{880000,4}, //{microvolt, in}
                                           {750000,6},
                                           {500000,8},
                                           {400000,10},
                                           {350000,12},
                                           {290000,14},
                                           {220000,16},
                                           {200000,18},
                                           {180000,20},
                                           {170000,22},
                                           {150000,24},
                                           {140000,26},
                                           {130000,28},
                                           {120000,30},
                                           {0,0}};


void adcInit()
{
    ADC_Params adc_params;
    ADC_Params_init(&adc_params);
    adc = ADC_open(CONFIG_ADC_0, &adc_params);
    if (adc == NULL) ERROR;
}

void pollSensor()
{
    static int total = 0, count = 0, avg = 0;
    uint16_t adcValue;
    if(ADC_convert(adc, &adcValue) != ADC_STATUS_SUCCESS) ERROR;
    uint32_t adcValueMicroVolt = ADC_convertRawToMicroVolts(adc, adcValue);
    int result = conversion(adcValueMicroVolt);
    total+=result;
    count++;
    if(count == 5)
    {
        avg = total/count;
        total = 0;
        count = 0;
        sendMsgToNaviQFromISR(SENSOR, avg);
        MQTTMsg msg = {.type = JSON_TYPE_DEBUG, .value = avg};
        sendMsgToMQTTQFromISR(msg);
    }
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
    return sensorConv;
}


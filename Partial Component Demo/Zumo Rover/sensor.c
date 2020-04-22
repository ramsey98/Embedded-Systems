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
                                           {360000,12},
                                           {320000,14},
                                           {280000,16},
                                           {260000,18},
                                           {240000,20},
                                           {230000,22},
                                           {220000,24},
                                           {210000,26},
                                           {200000,28},
                                           {170000,30},
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
        if(avg != 0)
        {
            sendMsgToNaviQFromISR(SENSOR, avg);
            MQTTMsg msg = {.topic = JSON_TOPIC_DEBUG, .type = JSON_SENSOR, .value = avg};
            sendMsgToMQTTQFromISR(msg);
        }
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


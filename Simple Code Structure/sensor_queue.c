/*
 * sensor_queue.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#include "sensor_queue.h"


int sendTimeMsgToQ1(unsigned int timeVal)
{
    int success = 1;
    return success;
}

int sendSensorMsgToQ1(int mmDist)
{
    int success = 1;
    ADC_Handle   adc;
    ADC_Params   params;
    int_fast16_t res;

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_ADC_0, &params);

    if (adc == 0) {
        success = 0;
    }

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue);
    if (res == ADC_STATUS_SUCCESS)
    {
        adcValueMicroVolt = ADC_convertRawToMicroVolts(adc, adcValue);
    }
    else
    {
        success = 0; //conversion failed
    }

    ADC_close(adc);

    return success;
}



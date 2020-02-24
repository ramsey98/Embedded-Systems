/*
 * Main.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */
#include "mqtt_client.h"
#include "debug.h"
#include "pixy.h"
#include "capture.h"
#include "motors.h"
#include "sensor.h"
#include "test.h"
#include <pthread.h>

#define THREADSTACKSIZE (1024)

void *mainThread(void *arg0)
{
    pthread_t capture, UARTRx, UARTTx, motors, test, UARTOut;//, mqtt; //pixy, sensor;
    pthread_attr_t attrs;
    struct sched_param  priParam;
    int retc;
    int detachState;

    ADC_init();
    SPI_init();
    Timer_init();
    Capture_init();
    GPIO_init();
    UART_init();

    //createSensorQueue();
    //createPixyQueue();
    createCaptureQueue();
    createMotorsQueue();
    createUARTTxQueue();
    createUARTRxQueue();
    //createMQTTQueue();

    dbgUARTInit();
    dbgGPIOInit();

    pthread_attr_init(&attrs);
    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0)
    {
        ERROR;
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0)
    {
        ERROR;
    }

    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);
    /*
    retc = pthread_create(&sensor, &attrs, sensorThread, NULL);
    if (retc != 0)
    {
        halt();
    }

    retc = pthread_create(&pixy, &attrs, pixyThread, NULL);
    if (retc != 0)
    {
        halt();
    }
    */

    retc = pthread_create(&capture, &attrs, captureThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }

    retc = pthread_create(&motors, &attrs, motorsThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }

    retc = pthread_create(&UARTTx, &attrs, UARTTxThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }

    retc = pthread_create(&UARTRx, &attrs, UARTRxThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }

    retc = pthread_create(&UARTOut, &attrs, UARTOutThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }

    /*
    retc = pthread_create(&mqtt, &attrs, commThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }
    */
    retc = pthread_create(&test, &attrs, testThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }
    return(NULL);
}





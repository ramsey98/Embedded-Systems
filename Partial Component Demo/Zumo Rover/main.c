/*
 * Main.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */
#include "debug.h"
#include "pixy.h"
#include "capture.h"
#include "motors.h"
#include "sensor.h"
#include <pthread.h>

#define THREADSTACKSIZE (1024)

extern void *commThread(void * args);

void runTests()
{
    sendMsgToUARTTxQ(INIT_CONTROLLER);
    sendMsgToUARTTxQ(GET_FIRMWARE);
    sendMsgToUARTTxQ(GET_CONFIG);
    sendMsgToUARTTxQ(DEVICE_ID);
    sendMsgToUARTTxQ(GET_CONFIG);
    sendMsgToUARTTxQ(PWM_PARAM);
    sendMsgToUARTTxQ(GET_CONFIG);
    sendMsgToUARTTxQ(SHUTDOWN_ON_ERROR);
    sendMsgToUARTTxQ(GET_CONFIG);
    sendMsgToUARTTxQ(SERIAL_TIMEOUT);

    /*
    //Move forward & reverse
    sendMsgToUARTTxQ(M0_FORWARD);
    sendMsgToUARTTxQ(0xFF);
    sendMsgToUARTTxQ(M1_FORWARD);
    sendMsgToUARTTxQ(0xFF);
    sleep(3);
    sendMsgToUARTTxQ(M0_REVERSE);
    sendMsgToUARTTxQ(0xFF);
    sendMsgToUARTTxQ(M1_REVERSE);
    sendMsgToUARTTxQ(0xFF);
    */

    /*
    //turn clockwise
    sendMsgToUARTTxQ(M0_FORWARD);
    sendMsgToUARTTxQ(0xFF);
    sendMsgToUARTTxQ(M1_REVERSE);
    sendMsgToUARTTxQ(0xFF);
    sleep(3);
    sendMsgToUARTTxQ(M0_FORWARD);
    sendMsgToUARTTxQ(0x00);
    sendMsgToUARTTxQ(M1_REVERSE);
    sendMsgToUARTTxQ(0x00);
    */
}

void *mainThread(void *arg0)
{
    pthread_t capture, motors, UARTRx, UARTTx, UARTOut, mqtt; //pixy, sensor
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

    dbgUARTInit();
    dbgGPIOInit();

    //createSensorQueue();
    //createPixyQueue();
    createCaptureQueue();
    createSpeedQueue();
    createUARTTxQueue();
    createUARTRxQueue();

    pthread_attr_init(&attrs);
    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0)
    {
        halt();
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0)
    {
        halt();
    }

    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);
    /*
    retc = pthread_create(&sensor, &attrs, sensorThread, NULL);
    if (retc != 0)
    {
        halt();
    }
    */
    /*
    retc = pthread_create(&pixy, &attrs, pixyThread, NULL);
    if (retc != 0)
    {
        halt();
    }
    */
    retc = pthread_create(&capture, &attrs, captureThread, NULL);
    if (retc != 0)
    {
        halt();
    }

    retc = pthread_create(&motors, &attrs, motorsThread, NULL);
    if (retc != 0)
    {
        halt();
    }

    retc = pthread_create(&UARTTx, &attrs, UARTTxThread, NULL);
    if (retc != 0)
    {
        halt();
    }

    retc = pthread_create(&UARTRx, &attrs, UARTRxThread, NULL);
    if (retc != 0)
    {
        halt();
    }

    retc = pthread_create(&UARTOut, &attrs, UARTOutThread, NULL);
    if (retc != 0)
    {
        halt();
    }

    retc = pthread_create(&mqtt, &attrs, commThread, NULL);
    if (retc != 0)
    {
        halt();
    }
    //runTests();
    return(NULL);
}





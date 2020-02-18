/*
 * Main.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Holden Ramsey
 */
#include "debug.h"
#include "sensor_state.h"
#include "timerone.h"
#include "timertwo.h"
#include "pixy.h"
#include "capture.h"
#include "motors.h"
#include "capture_queue.h"
#include "pixy_queue.h"
#include "sensor_queue.h"
#include "speed_queue.h"
#include <pthread.h>

#define THREADSTACKSIZE (1024)

void *sensorThread(void *arg0)
{
    adcInit();
    timerOneInit();
    timerTwoInit();
    dbgOutputLoc(ENTER_TASK);
    SENSOR_DATA curState;
    curState.state = Init;
    int timeInc = 0;
    int sensorVal = 0;
    int success = fsm(&curState, timeInc, sensorVal);
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        success = fsm(&curState, timeInc, sensorVal);
        if(success == -1 || received == -1)
        {
            halt();
        }
    }
}

void *captureThread(void *arg0)
{
    capture0Init();
    capture1Init();
    dbgOutputLoc(ENTER_TASK);
    uint32_t leftFreq = 0;
    uint32_t rightFreq = 0;
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromCapQ(&leftFreq, &rightFreq);
        if(received == -1)
        {
            halt();
        }
    }
}

void *motorsThread(void *arg0)
{
    motorsUARTInit();
    dbgOutputLoc(ENTER_TASK);
    uint16_t speed;
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromSpeedQ(&speed);
        if(received == -1)
        {
            halt();
        }
    }
}
/*
void *pixyThread(void *arg0)
{
    pixy_Init();
    dbgOutputLoc(ENTER_TASK);
    int received = 0;
    uint32_t value;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromSpeedQ(&value);
        if(received == -1)
        {
            halt();
        }
    }
}
*/
void *mainThread(void *arg0)
{
    pthread_t sensor, capture, motors; //pixy
    pthread_attr_t attrs;
    struct sched_param  priParam;
    int retc;
    int detachState;

    Timer_init();
    Capture_init();
    GPIO_init();
    UART_init();
    ADC_init();
    SPI_init();

    dbgUARTInit();
    dbgGPIOInit();

    createSensorQueue();
    createCaptureQueue();
    createSpeedQueue();
    //createPixyQueue();

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

    retc = pthread_create(&sensor, &attrs, sensorThread, NULL);
    if (retc != 0)
    {
        halt();
    }

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
    /*
    retc = pthread_create(&pixy, &attrs, pixyThread, NULL);
    if (retc != 0)
    {
        halt();
    }
    */
    vTaskStartScheduler();
    return(NULL);
}





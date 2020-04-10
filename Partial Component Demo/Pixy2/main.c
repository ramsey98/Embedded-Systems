
/*
 * Main.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Matthew Conway
 */
#include "debug.h"
#include "sensor_state.h"
#include "sensor_queue.h"
#include "timerone.h"
#include "timertwo.h"
#include "spi.h"
#include "pixy_state.h"
#include "pixy_queue.h"
#include "spi_thread.h"
#include "distance_thread.h"
#include <pthread.h>
#include "uart_term.h"
#include "config.h"
#include "debug_queue.h"

//extern void runMQTT();

#define THREADSTACKSIZE (1024)

void *mainThread(void *arg0)
{
    pthread_t spi, distance, UARTDebug;//, mqtt; //pixy, sensor;
    pthread_attr_t attrs;
    struct sched_param  priParam;
    int retc;
    int detachState;
    UART_Handle tUartHndl;

    ADC_init();
    SPI_init();
    Timer_init();
    GPIO_init();
    UART_init();

    dbgGPIOInit();
    tUartHndl = InitTerm();
    dbgUARTInit(tUartHndl);

    createPixyQueue();
    createDistanceQueue();
    createUARTDebugQueue();
    createMQTTQueue();
    createConfigQueue();

    adcInit();
    spiInit();
    runMQTT();
    dbgOutputLoc(ENTER_TASK);

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

    retc = pthread_create(&spi, &attrs, spiThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }

    retc = pthread_create(&distance, &attrs, distanceThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }

    if(pthread_create(&UARTDebug, &attrs, UARTDebugThread, NULL) != 0) ERROR;

    timerOneInit();
    timer100MSInit();

    return (NULL);

}

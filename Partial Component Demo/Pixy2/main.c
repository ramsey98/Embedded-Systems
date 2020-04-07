
/*
 * Main.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
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

#define THREADSTACKSIZE (1024)

void *mainThread(void *arg0)
{
    pthread_t spi, distance;//, mqtt; //pixy, sensor;
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

    timerOneInit();
    timerTwoInit();
    dbgGPIOInit();
    adcInit();
    spiInit();
    createPixyQueue();
    createDistanceQueue();
    dbgOutputLoc(ENTER_TASK);

    /*Configure the UART                                                     */
    tUartHndl = InitTerm();
    /*remove uart receive from LPDS dependency                               */
    UART_control(tUartHndl, UART_CMD_RXDISABLE, NULL);
    dbgUARTInit(tUartHndl);

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

    return (NULL);

}

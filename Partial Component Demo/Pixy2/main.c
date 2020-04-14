
/*
 * Main.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Matthew Conway
 */

#include <ti/drivers/Capture.h>
#include "ti_drivers_config.h"

//general file imports
#include "debug.h"
#include "timerone.h"
#include "timertwo.h"
#include "timertrigger.h"
#include "config.h"
#include "uart_term.h"

//queue imports
#include "pixy_queue.h"
#include "sensor_queue.h"
#include "debug_queue.h"

//thread imports
#include "spi_thread.h"
#include "distance_thread.h"
#include "sensor_thread.h"
#include <pthread.h>

extern void runMQTT();

#define THREADSTACKSIZE (1024)

void *mainThread(void *arg0)
{
    pthread_t spi, distance, sensor;//, mqtt; //pixy, sensor;
    pthread_attr_t attrs;
    struct sched_param  priParam;
    int detachState;
    UART_Handle tUartHndl;

    ADC_init();
    SPI_init();
    Timer_init();
    GPIO_init();
    UART_init();
    Capture_init();

    dbgGPIOInit();
    tUartHndl = InitTerm();
    dbgUARTInit(tUartHndl);

    createPixyQueue();
    createDistanceQueue();
    createDebugQueue();
    createMQTTQueue();
    createConfigQueue();
    createSensorQueue();

    adcInit();
    spiInit();
    runMQTT();
    dbgOutputLoc(ENTER_TASK);

    pthread_attr_init(&attrs);
    detachState = PTHREAD_CREATE_DETACHED;
    if(pthread_attr_setdetachstate(&attrs, detachState) != 0) ERROR;
    if(pthread_attr_setstacksize(&attrs, THREADSTACKSIZE) != 0) ERROR;
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    if(pthread_create(&spi, &attrs, spiThread, NULL) != 0) ERROR;
    if(pthread_create(&distance, &attrs, distanceThread, NULL) != 0) ERROR;
    if(pthread_create(&sensor, &attrs, sensorThread, NULL) != 0) ERROR;
    //if(pthread_create(&UARTDebug, &attrs, UARTDebugThread, NULL) != 0) ERROR;  //todo, this is currently overlapped with MQTT debug queu

    timerOneInit();
    timer100MSInit();
    timerTriggerInit();

    return (NULL);

}

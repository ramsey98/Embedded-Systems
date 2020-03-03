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
#include "tests.h"
#include "motor_controller_queue.h"
#include "motor_controller_thread.h"
#include <pthread.h>
#include <stdio.h>
#include <ti/drivers/UART.h>

#define THREADSTACKSIZE (1024)

//void *mainThread(void *arg0)
//{
//    int retc;
//
//
//    pthread_t motors, tests;
//    pthread_attr_t attrs;
//    struct sched_param  priParam;
//
//    Board_init();
//    dbgUARTInit();
//    dbgGPIOInit();
//    adcInit();
//
//    createSensorQueue();
//    createMotorControllerQueue();
//
//    Timer_init();
//    timerOneInit();
//    timerTwoInit();
//
//    dbgOutputLoc(ENTER_TASK);
//    SENSOR_DATA curState;
//    curState.state = Init;
//    int timeInc = 0;
//    int sensorVal = 0;
//    int success = fsm(&curState, timeInc, sensorVal);
////    int received = 0;
//    dbgOutputLoc(WHILE1);
//
//
////    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
////    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
////    GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_OFF);
////    sleep(1);
////    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
////    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);
////    GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_ON);
////    sleep(1);
//
////    UART_Handle uart;
////    initialize(&uart);
////    int i;
////    for (i = 0; i < 10; i++) {
////        GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
////        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
////        sleep(1);
//////        sendMsgToMCQ(0x00);
////        GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);
////        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
////        sleep(1);
////        uint8_t checksum = (0x10 + 0x04 + 0x0F) & 0b01111111;
////        uint8_t buffer[] = {0x10, 0x04, 0x0F, checksum};
////        UART_write(uart, *buffer, sizeof(buffer));
////    }
//
//
//
//    pthread_attr_init(&attrs);
//    priParam.sched_priority = 1;
//    pthread_attr_setschedparam(&attrs, &priParam);
//    int detachState = PTHREAD_CREATE_DETACHED;
//    retc = pthread_attr_setdetachstate(&attrs, detachState);
//    if (retc != 0)
//    {
//        ERROR;
//    }
//
//    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
//    if (retc != 0)
//    {
//        ERROR;
//    }
//
//
//
////    retc = pthread_create(&motors, &attrs, mcThread, NULL);
////    if (retc != SUCCESS)
////    {
////        ERROR;
////    }
////    while(1)
////    {
////        received = receiveFromQ1(&timeInc, &sensorVal);
////        success = fsm(&curState, timeInc, sensorVal);
////        if(success == -1 || received == -1)
////        {
////            ERROR;
////        }
////    }
//    retc = pthread_create(&tests, &attrs, testThread, NULL);
//    if (retc != 0)
//    {
//        ERROR;
//    }
//    return(NULL);
//}
//

void *mainThread(void *arg0)
{
    pthread_t capture, UARTTx, motors, test, UARTDebug;//, mqtt; //pixy, sensor;
    pthread_attr_t attrs;
    struct sched_param  priParam;
    int retc;
    int detachState;

    ADC_init();
//    SPI_init();
    Timer_init();
//    Capture_init();
    GPIO_init();
    UART_init();

    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_OFF);

    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
    sleep(1);
    moveRover(0x4F);
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_OFF);

    sleep(1);

    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);

    //createSensorQueue();
    //createPixyQueue();
//    createCaptureQueue();
//    createMotorControllerQueue();
//    createUARTTxQueue();
//    createUARTDebugQueue();
//    //createMQTTQueue();
//
//    dbgUARTInit();
//    dbgGPIOInit();
//
//    pthread_attr_init(&attrs);
//    detachState = PTHREAD_CREATE_DETACHED;
//    retc = pthread_attr_setdetachstate(&attrs, detachState);
//    if (retc != 0)
//    {
//        ERROR;
//    }
//
//    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
//    if (retc != 0)
//    {
//        ERROR;
//    }
//
//    priParam.sched_priority = 1;
//    pthread_attr_setschedparam(&attrs, &priParam);
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

//    retc = pthread_create(&capture, &attrs, captureThread, NULL);
//    if (retc != 0)
//    {
//        ERROR;
//    }

//
//    retc = pthread_create(&UARTTx, &attrs, UARTTxThread, NULL);
//    if (retc != 0)
//    {
//        ERROR;
//    }
//
//    retc = pthread_create(&UARTDebug, &attrs, UARTDebugThread, NULL);
//    if (retc != 0)
//    {
//        ERROR;
//    }

    /*
    retc = pthread_create(&mqtt, &attrs, commThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }
    */
//    retc = pthread_create(&motors, &attrs, &mcThread, NULL);
//    if (retc != 0)
//    {
//        ERROR;
//    }
    retc = pthread_create(&test, &attrs, testThread, NULL);
    if (retc != 0)
    {
        ERROR;
    }
    return(NULL);
}








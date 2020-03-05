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
#include "motor_encoder_queue.h"
#include "motor_controller_thread.h"
#include "motor_encoder_thread.h"
#include <pthread.h>
#include <stdio.h>
#include <ti/drivers/UART.h>

#define THREADSTACKSIZE (1024)

void *mainThread(void *arg0) {
    pthread_t motors, test, encoders;
    pthread_attr_t attrs;

    int retc;
    int detachState;

    ADC_init();
    SPI_init();
    Timer_init();
    GPIO_init();
    UART_init();

    createMotorControllerQueue();
    createEncoderQueue();

    pthread_attr_init(&attrs);
    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        ERROR;
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        ERROR;
    }

    retc = pthread_create(&motors, &attrs, &mcThread, NULL);
    if (retc != 0) {
        ERROR;
    }

    retc = pthread_create(&encoders, &attrs, &encoderThread, NULL);
    if (retc != 0) {
        ERROR;
    }

    retc = pthread_create(&test, &attrs, testThread, NULL);
    if (retc != 0) {
        ERROR;
    }

    return(NULL);
}








/*
 * sensor_state.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef SENSOR_STATE_H_
#define SENSOR_STATE_H_

#include <pthread.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"


UART_Params_init(&uartParams);
uartParams.writeDataMode = UART_DATA_BINARY;
uartParams.readDataMode = UART_DATA_BINARY;
uartParams.readReturnMode = UART_RETURN_FULL;
uartParams.readEcho = UART_ECHO_OFF;
uartParams.baudRate = 115200;

uart = UART_open(CONFIG_UART_0, &uartParams);

const char  uartOut[];
UART_Handle uart;
UART_Params uartParams;

typedef enum
{
    Init,
    WaitingForTime1,
    WaitingForTime2,
    WaitingForTime3,
} STATES;

//curTime needs to be based on timer!!!!
int curTime, sensorTotal, sensorCount, sensorAvg;

int fsm(STATES curState, int timeInc, int sensorVal);

#endif /* SENSOR_STATE_H_ */

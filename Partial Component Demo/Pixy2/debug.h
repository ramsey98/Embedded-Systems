/*
 * debug.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"
#include "FreeRTOS.h"
#include "task.h"

#define DBG_HALT 0
#define ENTER_TASK 1
#define WHILE1 2
#define BEFORE_RECEIVE_QUEUE 3
#define AFTER_RECEIVE_QUEUE 4
#define ENTER_ISR_TIMER1 5
#define LEAVE_ISR_TIMER1 6
#define BEFORE_SEND_QUEUE_ISR_TIMER1 7
#define AFTER_SEND_QUEUE_ISR_TIMER1 8
#define ENTER_ISR_TIMER2 9
#define LEAVE_ISR_TIMER2 10
#define BEFORE_SEND_QUEUE_ISR_TIMER2 11
#define AFTER_SEND_QUEUE_ISR_TIMER2 12
#define SPI_INIT 13
#define SPI_BEGIN_TRANSFER 14
#define SPI_CALLBACK 15
#define SPI_FAIL_TRANSFER 16
#define SPI_SUCCESS_TRANSFER 17

#define DBG_ERROR_LED_TIME (250000)


void dbgGPIOInit();
void dbgUARTInit();
void dbgUARTVal(unsigned char outVal);
void dbgUARTStr(char * uartOut);
void dbgUARTNum(int outVal);
void dbgOutputLoc(unsigned int outVal);
void halt();

#endif /* DEBUG_H_ */

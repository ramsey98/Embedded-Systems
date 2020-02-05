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

#define DBG_HALT (255)
#define ENTER_TASK (1)
#define WHILE1 (2)
#define BEFORE_SEND_QUEUE (3)
#define BEFORE_RECEIVE_QUEUE (4)
#define AFTER_SEND_QUEUE (5)
#define AFTER_RECEIVE_QUEUE (6)
#define ENTER_ISR (7)
#define LEAVE_ISR (8)
#define BEFORE_SEND_QUEUE_ISR (9)
#define BEFORE_RECEIVE_QUEUE_ISR (10)
#define AFTER_SEND_QUEUE_ISR (11)
#define AFTER_RECEIVE_QUEUE_ISR (12)

void dbgGPIOInit();
void dbgUARTInit();
void dbgUARTVal(unsigned char outVal);
void dbgUARTStr(char uartOut[]);
void dbgOutputLoc(unsigned int outVal);
void halt();

UART_Handle uart;
UART_Params uartParams;

#endif /* DEBUG_H_ */

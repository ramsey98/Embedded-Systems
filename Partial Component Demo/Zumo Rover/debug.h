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
#include "uart_term.h"

#define STRLEN 10

#define DBG_HALT (0)
#define ENTER_TASK (1)
#define WHILE1 (2)
#define BEFORE_SEND_QUEUE (3)
#define AFTER_SEND_QUEUE (4)
#define BEFORE_RECEIVE_QUEUE (5)
#define AFTER_RECEIVE_QUEUE (6)
#define ENTER_ISR_TIMER (7)
#define LEAVE_ISR_TIMER (8)
#define BEFORE_SEND_QUEUE_ISR_TIMER (9)
#define AFTER_SEND_QUEUE_ISR_TIMER (10)
#define MQTT_MSG_RECEIVED (11)
#define ENTER_SEND_STATS (12)
#define ENTER_SEND_DEBUG (13)
#define ENTER_SEND_ERROR (14)
#define ENTER_PIXY_FSM 15
#define LEAVE_PIXY_FSM 16

#define DBG_ERROR_LED_TIME (250000)
#define ERROR halt(__LINE__, __func__);

void dbgGPIOInit();
void dbgUARTInit(UART_Handle uartHandle);
void dbgUARTVal(unsigned char outVal);
void dbgUARTStr(const char* uartOut);
void dbgUARTNum(int outVal);
void dbgOutputLoc(unsigned int outVal);
void halt(int line, const char* func);

#endif /* DEBUG_H_ */

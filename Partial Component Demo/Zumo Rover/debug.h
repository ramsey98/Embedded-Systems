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

#define DBG_HALT (0x00)
#define ENTER_TASK (0x01)
#define WHILE1 (0x02)
#define BEFORE_SEND_QUEUE (0x03)
#define AFTER_SEND_QUEUE (0x04)
#define BEFORE_RECEIVE_QUEUE (0x05)
#define AFTER_RECEIVE_QUEUE (0x06)
#define ENTER_ISR_TIMER (0x07)
#define LEAVE_ISR_TIMER (0x08)
#define BEFORE_SEND_QUEUE_ISR_TIMER (0x09)
#define AFTER_SEND_QUEUE_ISR_TIMER (0x10)
#define BEFORE_SEND_QUEUE_ISR_TIMER2 (0x15)
#define AFTER_SEND_QUEUE_ISR_TIMER2 (0x16)

#define DBG_ERROR_LED_TIME (250000)
#define ERROR halt(__LINE__, __func__);

void dbgGPIOInit();
void dbgUARTInit();
void dbgUARTVal(unsigned char outVal);
void dbgUARTStr(const char* uartOut);
void dbgUARTNum(int outVal);
void dbgOutputLoc(unsigned int outVal);
void halt(int line, const char* func);

#endif /* DEBUG_H_ */

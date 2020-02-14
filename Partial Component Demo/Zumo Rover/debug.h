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

#define DBG_HALT (0b00000000)
#define ENTER_TASK (0b00000001)
#define WHILE1 (0b00000010)
//#define BEFORE_SEND_QUEUE (0b00000100)
//#define AFTER_SEND_QUEUE (0b00000101)
#define BEFORE_RECEIVE_QUEUE (0b0000100)
#define AFTER_RECEIVE_QUEUE (0b00000101)
#define ENTER_ISR_TIMER1 (0b00001000)
#define LEAVE_ISR_TIMER1 (0b00001001)
#define BEFORE_SEND_QUEUE_ISR_TIMER1 (0b00001010)
#define AFTER_SEND_QUEUE_ISR_TIMER1 (0b00001011)
//#define BEFORE_RECEIVE_QUEUE_ISR (0b00001010)
//#define AFTER_RECEIVE_QUEUE_ISR (0b00001100)
#define ENTER_ISR_TIMER2 (0b00010000)
#define LEAVE_ISR_TIMER2 (0b00010001)
#define BEFORE_SEND_QUEUE_ISR_TIMER2 (0b00010010)
#define AFTER_SEND_QUEUE_ISR_TIMER2 (0b00010011)

#define DBG_ERROR_LED_TIME (250000)


void dbgGPIOInit();
void dbgUARTInit();
void dbgUARTVal(unsigned char outVal);
void dbgUARTStr(char * uartOut);
void dbgUARTNum(int outVal);
void dbgOutputLoc(unsigned int outVal);
void halt();

#endif /* DEBUG_H_ */

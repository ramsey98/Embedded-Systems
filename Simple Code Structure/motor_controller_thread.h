/*
 * motor_controller_thread.h
 *
 *  Created on: Feb 24, 2020
 *      Author: jared
 */

#ifndef MOTOR_CONTROLLER_THREAD_H_
#define MOTOR_CONTROLLER_THREAD_H_

#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "debug.h"
#include "UARTDebug_queue.h"
#include "motor_controller_queue.h"
#include "UARTTx_queue.h"
#include <stdio.h>
#include <unistd.h>

void initWheels(MoveCommand * wheel_0, MoveCommand * wheel_1, MoveCommand * wheel_2);
void *mcThread(void *arg0);
void initialize();
void moveRover(uint16_t command);
void *UARTDebugThread(void *arg0);
void *UARTTxThread(void *arg0);


#endif /* MOTOR_CONTROLLER_THREAD_H_ */

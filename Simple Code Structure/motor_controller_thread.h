/*
 * motor_controller_thread.h
 *
 *  Created on: Feb 24, 2020
 *      Author: jared
 */

#ifndef MOTOR_CONTROLLER_THREAD_H_
#define MOTOR_CONTROLLER_THREAD_H_

#include <ti/drivers/UART.h>
#include "debug.h"
#include "motor_controller_queue.h"
#include <stdio.h>
#include <unistd.h>

void initWheels(MoveCommand * wheel_0, MoveCommand * wheel_1, MoveCommand * wheel_2);
void *mcThread(void *arg0);
void initialize(UART_Handle * uart);
void moveRover(UART_Handle uart, uint16_t command);


#endif /* MOTOR_CONTROLLER_THREAD_H_ */

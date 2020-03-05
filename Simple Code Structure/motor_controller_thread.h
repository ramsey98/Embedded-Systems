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

#define STOP    0x0
#define TURN    0x1
#define MOVE    0x2
#define SINGLE  0x3

void initWheels(MoveCommand * wheel_0, MoveCommand * wheel_1, MoveCommand * wheel_2);
void *mcThread(void *arg0);
void initialize(UART_Handle * motors_uart);
void handleMovementType(UART_Handle motors_uart, uint16_t command);
void moveWheel(UART_Handle motors_uart, uint8_t address, uint8_t direction, uint8_t magnitude);
void moveRover(UART_Handle motors_uart, uint16_t command);
void moveSingle(UART_Handle motors_uart, uint16_t command);
void stopRover(UART_Handle motors_uart);
void turnRover(UART_Handle motors_uart, uint16_t command);
void moveAllMotorsSame(UART_Handle motors_uart, uint8_t magnitude, uint8_t direction);
void *UARTDebugThread(void *arg0);
void *UARTTxThread(void *arg0);


#endif /* MOTOR_CONTROLLER_THREAD_H_ */

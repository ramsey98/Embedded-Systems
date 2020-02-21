/*
 * motors.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include "debug.h"
#include "motors_queue.h"
#include "UARTTx_queue.h"
#include "UARTRx_queue.h"
#include "motors_state.h"

void motorsUARTInit();
void writeMotors(unsigned char outVal);
void *motorsThread(void *arg0);
void *UARTOutThread(void *arg0);
void *UARTRxThread(void *arg0);
void *UARTTxThread(void *arg0);

#endif /* MOTORS_H_ */

/*
 * UARTRx_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef DEBUG_QUEUE_H_
#define DEBUG_QUEUE_H_

#include "debug.h"
#include "queue.h"

#define PIXY_COLOR 1
#define PIXY_DISTANCE 2
#define SENSOR 3

#define UARTSHIFT (32)
void sendMsgToDebugQ(int msg);
void receiveFromDebugQ(int *received);
void createDebugQueue();
void *UARTDebugThread(void *arg0);

#endif /* DEBUG_QUEUE_H_ */

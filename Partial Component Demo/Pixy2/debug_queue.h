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

#define UARTSHIFT (32)
void sendMsgToUARTDebugQ(int msg);
void receiveFromUARTDebugQ(int *received);
void createUARTDebugQueue();
void *UARTDebugThread(void *arg0);

#endif /* DEBUG_QUEUE_H_ */

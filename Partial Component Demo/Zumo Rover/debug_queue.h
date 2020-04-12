/*
 * UARTRx_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef DEBUG_QUEUE_H_
#define DEBUG_QUEUE_H_

#include <navi_queue.h>
#include "debug.h"
#include "queue.h"

#define UARTSHIFT (32)
void sendMsgToUARTDebugQ(uint32_t type, uint32_t value);
void sendMsgToUARTDebugQFromISR(uint32_t type, uint32_t value);
void receiveFromUARTDebugQ(uint32_t * type, uint32_t * value);
void createUARTDebugQueue();
void *UARTDebugThread(void *arg0);

#endif /* DEBUG_QUEUE_H_ */

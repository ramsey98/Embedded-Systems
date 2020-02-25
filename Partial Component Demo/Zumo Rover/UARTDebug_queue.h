/*
 * UARTRx_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef UARTDEBUG_QUEUE_H_
#define UARTDEBUG_QUEUE_H_

#include "debug.h"
#include "queue.h"

int sendLeftCapMsgToUARTDebugQ(uint16_t value);
int sendRightCapMsgToUARTDebugQ(uint16_t value);
int receiveFromUARTDebugQ(uint16_t * type, uint16_t * value);
void createUARTDebugQueue();



#endif /* UARTDEBUG_QUEUE_H_ */

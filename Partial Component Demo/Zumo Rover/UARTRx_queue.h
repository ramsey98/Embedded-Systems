/*
 * UARTRx_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef UARTRX_QUEUE_H_
#define UARTRX_QUEUE_H_

#include "debug.h"
#include "queue.h"

int sendMsgToUARTRxQ(uint8_t value);
int receiveFromUARTRxQ(uint8_t * value);
void createUARTRxQueue();



#endif /* UARTRX_QUEUE_H_ */

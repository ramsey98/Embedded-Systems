/*
 * UARTTx_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef UARTTX_QUEUE_H_
#define UARTTX_QUEUE_H_

#include "debug.h"
#include "queue.h"

int sendMsgToUARTTxQ(uint8_t value);
int receiveFromUARTTxQ(uint8_t * value);
void createUARTTxQueue();

#endif /* UARTTX_QUEUE_H_ */

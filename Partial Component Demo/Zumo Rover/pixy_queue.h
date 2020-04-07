/*
 * pixy_queue.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef PIXY_QUEUE_H_
#define PIXY_QUEUE_H_

#include "debug.h"
#include "queue.h"

void sendMsgToPixyQ(uint8_t type);
void sendMsgToPixyQFromISR(uint8_t type);
void receiveFromPixyQ(uint8_t *type);
void createPixyQueue();

#endif /* PIXY_QUEUE_H_ */

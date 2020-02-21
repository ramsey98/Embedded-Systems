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

int sendMsgToPixyQ(uint32_t value);
int receiveFromPixyQ(uint32_t * value);
void createPixyQueue();

#endif /* PIXY_QUEUE_H_ */

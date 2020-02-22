/*
 * pixy_queue.h
 *
 *  Created on: Feb 22, 2020
 *      Author: Matthew
 */

#ifndef PIXY_QUEUE_H_
#define PIXY_QUEUE_H_

#include "debug.h"
#include "queue.h"

#define FMASK (0xffffffff)
#define SHIFT (32)

int sendPixyToQ1(char *);
int receiveFromPixyQ1(char *);
void createPixyQueue();

#endif /* PIXY_QUEUE_H_ */

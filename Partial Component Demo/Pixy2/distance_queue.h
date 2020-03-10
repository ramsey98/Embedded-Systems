/*
 * distance_queue.h
 *
 *  Created on: Mar 4, 2020
 *      Author: Matthew
 */

#ifndef DISTANCE_QUEUE_H_
#define DISTANCE_QUEUE_H_

#include "debug.h"
#include "queue.h"
#include "spi.h"

void createDistanceQueue();
int sendBlockMsgToDistanceQ1(DISTANCE_DATA *data);
int recieveMsgFromDistanceQ1(DISTANCE_DATA *data, int * transfer);
#endif /* DISTANCE_QUEUE_H_ */

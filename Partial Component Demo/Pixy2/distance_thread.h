/*
 * distance_thread.h
 *
 *  Created on: Mar 4, 2020
 *      Author: Matthew
 */

#ifndef DISTANCE_THREAD_H_
#define DISTANCE_THREAD_H_

#include "debug.h"
#include "spi.h"
#include "distance_queue.h"

int findDistances(DISTANCE_DATA *data, int * transfer);
int findDistance(BLOCK_DATA *data);
void *distanceThread(void *arg0);


#endif /* DISTANCE_THREAD_H_ */

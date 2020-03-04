/*
 * distance_thread.h
 *
 *  Created on: Mar 4, 2020
 *      Author: Matthew
 */

#ifndef DISTANCE_THREAD_H_
#define DISTANCE_THREAD_H_

#define FOCAL_PIXELS_30 30
#define FOCAL_PIXELS_50 20
#define FOCAL_PIXELS_70 16
#define FOCAL_PIXELS_90 14
#define FOCAL_LENGTH_30 195.725
#define FOCAL_LENGTH_50 224.972
#define FOCAL_LENGTH_70 251.969
#define FOCAL_LENGTH_90 283.464
#define FOCAL_PIXELS_30_Y 34.5
#define FOCAL_PIXELS_50_Y 32
#define FOCAL_PIXELS_70_Y 19
#define FOCAL_PIXELS_90_Y 17
#define FOCAL_LENGTH_30_Y 232.845
#define FOCAL_LENGTH_50_Y 359.95
#define FOCAL_LENGTH_70_Y 299.21
#define FOCAL_LENGTH_90_Y 344.206
#define EGG_WIDTH 4.445

#include "debug.h"
#include "spi.h"
#include "distance_queue.h"

int findDistances(DISTANCE_DATA *data, int * transfer);
int findDistance(BLOCK_DATA *data);
void *distanceThread(void *arg0);


#endif /* DISTANCE_THREAD_H_ */

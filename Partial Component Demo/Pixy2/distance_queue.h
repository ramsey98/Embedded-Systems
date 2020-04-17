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

typedef struct {
    uint8_t colorCode;
    uint16_t xPos;
    uint16_t xPixels;
    int angle;
    uint8_t distance;
} DISTANCE_BLOCK;

typedef struct
{
    DISTANCE_BLOCK blocks[MAX_BLOCKS];
    uint8_t blockCount;
} DISTANCE_DATA;



void createDistanceQueue();
int sendBlockMsgToDistanceQ1(DISTANCE_DATA *data);
int recieveMsgFromDistanceQ1(DISTANCE_DATA *data, int * transfer);
#endif /* DISTANCE_QUEUE_H_ */

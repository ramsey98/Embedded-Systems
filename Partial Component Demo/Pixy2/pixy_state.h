/*
 * pixy_state.h
 *
 *  Created on: Feb 26, 2020
 *      Author: Matthew
 */

#ifndef PIXY_STATE_H_
#define PIXY_STATE_H_

#include "debug.h"
#include "spi.h"
#include "distance_queue.h"
#include <string.h>

typedef enum
{
    PixyInit,
    PixySendVersion,
    PixyWaitingForVersion,
    PixyPan,
    PixyWaitingForTime1,
    PixyWaitingForBlockCount,
    PixyWaitingForBlocks
} PIXY_STATES;

typedef struct
{
    PIXY_STATES state;
    BLOCK_DATA blocks[MAX_BLOCKS];
    uint8_t rx_buffer[SPI_MSG_LENGTH];
    uint8_t tx_buffer[SPI_MSG_LENGTH];
    int curTime;
    uint8_t blockCount;
    int xPan;
    int yPan;
} PIXY_DATA;

int printState(DISTANCE_DATA *curState);
void initDistanceData(DISTANCE_DATA *d, PIXY_DATA *p);
int pixyFsm(PIXY_DATA *curState, int *timeInc, int *complete, int *sendInc);
#endif /* PIXY_STATE_H_ */

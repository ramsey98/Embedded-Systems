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
#include <string.h>

#define MAX_BLOCKS 10

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
    uint8_t colorCode;
    uint16_t xPos;
    uint8_t yPos;
    uint16_t xPixels;
    uint8_t yPixels;
    int angle;
    uint8_t trackIndex;
    uint8_t age;
} BLOCK_DATA;

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

int printState(PIXY_DATA *curState);
int pixyFsm(PIXY_DATA *curState, int *timeInc, int *complete, int *sendInc);
#endif /* PIXY_STATE_H_ */

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
    uint8_t blockCount;
} PIXY_DATA;

void initDistanceData(DISTANCE_DATA *d, PIXY_DATA *p);
void initBlockData(PIXY_DATA *curState);
void assignBlockData(PIXY_DATA *curState);
void checkSendDistanceData(int * sendInc, int * valid_data, PIXY_DATA * curState);
void checkSendVersionRequest(int * timeInc, PIXY_DATA *curState);
void checkSendVersionResponse(int * complete, PIXY_DATA *curState);
void checkSendConnectedBlockRequest(int * timeInc, PIXY_DATA * curState);
void checkSendConnectedBlockResponse(int * complete, int * valid_data, PIXY_DATA * curState);
int pixyFsm(PIXY_DATA *curState, int *timeInc, int *complete, int *sendInc);
#endif /* PIXY_STATE_H_ */

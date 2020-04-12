/*
 * pixy_state.h
 *
 *  Created on: Apr 2, 2020
 *      Author: Holden Ramsey
 */

#ifndef PIXY_STATE_H_
#define PIXY_STATE_H_

#include "debug.h"
#include "debug_queue.h"
#include "mqtt_queue.h"
#include "json_parse.h"
#include "pixy_queue.h"
#include <string.h>

#define MAX_BLOCKS (10)
#define SPI_MSG_LENGTH (128)
#define SPI_TX_MSG_VERSION (4)
#define SPI_TX_MSG_CONNECTED (6)
#define SPI_TX_MSG_COLOR (7)
#define SPI_TX_MSG_SERVOS (8)
#define CONNECTED_PACKET_LENGTH (14)
#define SPI_RATE (2000000)

#define PIXY_COMPLETE 1
#define PIXY_VERSION 2
#define PIXY_PAN 3
#define PIXY_COLOR 4
#define PIXY_TRANS 5

#define SYNC_FIRST 175
#define SYNC_SECOND 193
#define TYPE_VERSION 15
#define TYPE_COLOR 33

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
    uint8_t distance;
} BLOCK_DATA;

typedef enum
{
    PixyInit,
    PixyWaitingToSend,
    PixyWaitingForTransfer,
    PixyWaitingForBlocks
} PIXY_STATES;

typedef struct
{
    PIXY_STATES state;
    BLOCK_DATA blocks[MAX_BLOCKS];
    uint8_t rx_buffer[SPI_MSG_LENGTH];
    uint8_t tx_buffer[SPI_MSG_LENGTH];
    uint8_t blockCount;
    int xPan;
    int yPan;
} PIXY_DATA;

void pixy_fsm(PIXY_DATA *curState, uint8_t *type);
void processColor(PIXY_DATA *curState);
void processVersion(PIXY_DATA *curState);

#endif /* PIXY_STATE_H_ */

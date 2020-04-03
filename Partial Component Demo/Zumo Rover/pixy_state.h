/*
 * pixy_state.h
 *
 *  Created on: Apr 2, 2020
 *      Author: Holden Ramsey
 */

#ifndef PIXY_STATE_H_
#define PIXY_STATE_H_

#include "debug.h"
#include <string.h>

#define MAX_BLOCKS (10)
#define SPI_MSG_LENGTH (128)
#define SPI_TX_MSG_VERSION (4)
#define SPI_TX_MSG_CONNECTED (6)
#define SPI_TX_MSG_COLOR (7)
#define SPI_TX_MSG_SERVOS (8)
#define CONNECTED_PACKET_LENGTH (14)
#define CONNECTED_LENGTH_LOC (18)
#define SPI_RATE (2000000)
#define TIMEINCPIXY (1)
#define COMPLETEPIXY (2)
#define SENDINCPIXY (3)

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

typedef struct
{
    BLOCK_DATA blocks[MAX_BLOCKS];
    uint8_t blockCount;
} DISTANCE_DATA;

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

void initDistanceData(DISTANCE_DATA *d, PIXY_DATA *p);
void pixy_fsm(PIXY_DATA *curState, uint8_t *type);

void pixyGetVersionPacket(uint8_t *rx_buffer, uint8_t *tx_buffer);
void pixySetServos(uint8_t *rx_buffer, uint8_t *tx_buffer, int panX, int panY);
void pixySetColorPacket(uint8_t *rx_buffer, uint8_t *tx_buffer, uint8_t r, uint8_t g, uint8_t b);
void pixyGetConnectedBlocks(uint8_t *rx_buffer, uint8_t *tx_buffer);


#endif /* PIXY_STATE_H_ */

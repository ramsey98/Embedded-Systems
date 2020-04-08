/*
 * pixy_state.c
 *
 *  Created on: Apr 2, 2020
 *      Author: Holden Ramsey
 */

#include "pixy_state.h"

extern void pixyGetVersion(uint8_t *rx_buffer, uint8_t *tx_buffer);
extern void pixySetServos(uint8_t *rx_buffer, uint8_t *tx_buffer, uint16_t panX, uint16_t panY);
extern void pixySetColor(uint8_t *rx_buffer, uint8_t *tx_buffer, uint8_t r, uint8_t g, uint8_t b);
extern void pixyGetConnectedBlocks(uint8_t *rx_buffer, uint8_t *tx_buffer);

void processVersion(PIXY_DATA *curState)
{
    UART_PRINT("V");
    uint8_t version = 0;
    if(curState->rx_buffer[15+2] == 15)
    {
        version = curState->rx_buffer[15+8];
    }
    MQTTMsg msg = {JSON_TYPE_DEBUG, version};
    sendMsgToMQTTQ(msg);
}

void processColor(PIXY_DATA *curState)
{
    int i, loc;
    if(curState->rx_buffer[CONNECTED_LENGTH_LOC-1] == 33)
    {
        curState->blockCount = curState->rx_buffer[CONNECTED_LENGTH_LOC];
        if(curState->blockCount > 0)
        {
            sendMsgToUARTDebugQ(PIXY, curState->rx_buffer[CONNECTED_LENGTH_LOC]);
            loc = CONNECTED_LENGTH_LOC + 2;
            for(i = 0; i < curState->blockCount/CONNECTED_PACKET_LENGTH; i++)
            {
                curState->blocks[i].colorCode = curState->rx_buffer[loc+1];
                curState->blocks[i].xPos = (curState->rx_buffer[loc+2] << 8) | curState->rx_buffer[loc+3];
                curState->blocks[i].yPos = curState->rx_buffer[loc+5];
                curState->blocks[i].xPixels = (curState->rx_buffer[loc+6] << 8) | curState->rx_buffer[loc+7];
                curState->blocks[i].yPixels = curState->rx_buffer[loc+9];
                curState->blocks[i].angle = (curState->rx_buffer[loc+10] << 8) | curState->rx_buffer[loc+11];
                curState->blocks[i].trackIndex = curState->rx_buffer[loc+12];
                curState->blocks[i].age = curState->rx_buffer[loc+13];
                loc += CONNECTED_PACKET_LENGTH;
            }
        }
    }
}

void pixy_fsm(PIXY_DATA *curState, uint8_t *type)
{
    dbgOutputLoc(ENTER_PIXY_FSM);
    int i;
    static uint8_t prevType = 0;
    uint16_t panX = 1, panY = 1;
    switch (curState->state)
    {
        case PixyInit:
            curState->blockCount = 0;
            curState->xPan = 0;
            curState->yPan = 0;
            memset(curState->blocks, 0, MAX_BLOCKS * sizeof(BLOCK_DATA));
            for(i=0; i < MAX_BLOCKS; i++)
            {
                 curState->blocks[i].colorCode = i+1;
                 curState->blocks[i].xPos = 0;
                 curState->blocks[i].yPos = 0;
                 curState->blocks[i].xPixels = 0;
                 curState->blocks[i].yPixels = 0;
                 curState->blocks[i].angle = 0;
                 curState->blocks[i].trackIndex = 0;
                 curState->blocks[i].age = 0;
                 curState->blocks[i].distance = 0;
            }
            curState->state = PixyWaitingToSend;
            break;
        case PixyWaitingToSend:
            switch(*type)
            {
                case PIXY_VERSION:
                    sendMsgToUARTDebugQ(PIXY, 2);
                    pixyGetVersion(curState->rx_buffer, curState->tx_buffer);
                    break;
                case PIXY_PAN:
                    pixySetServos(curState->rx_buffer, curState->tx_buffer, panX, panY);
                    break;
                case PIXY_COLOR:
                    pixyGetConnectedBlocks(curState->rx_buffer, curState->tx_buffer);
                    break;
                default:
                    break;
            }
            prevType = *type;
            curState->state = PixyWaitingForTransfer;
            break;
        case PixyWaitingForTransfer:
            if(*type == PIXY_COMPLETE)
            {
                sendMsgToUARTDebugQ(PIXY, 3);
                switch(prevType)
                {
                    case PIXY_VERSION:
                        processVersion(curState);
                        break;
                    case PIXY_COLOR:
                        processColor(curState);
                        break;
                    default:
                        break;
                }
                curState->state = PixyWaitingToSend;
            }
            break;
        default:
            ERROR;
            break;
    }
    dbgOutputLoc(LEAVE_PIXY_FSM);
}


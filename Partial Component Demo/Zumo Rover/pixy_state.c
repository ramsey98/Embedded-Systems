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

static int panx = 0, pany = 0;

int processBuffer(PIXY_DATA *curState)
{
    int i;
    int check1 = 0, check2 = 0;
    int start = 0;
    for(i = 0; i < SPI_MSG_LENGTH; i++)
    {
        if(curState->rx_buffer[i] == SYNC_FIRST)
        {
            check1 = 1;
        }
        if(curState->rx_buffer[i] == SYNC_SECOND)
        {
            check2 = 1;
        }
        if(check1 & check2)
        {
            start = i+1;
            break;
        }
    }
    return start;
}

void processVersion(PIXY_DATA *curState)
{
    uint8_t version = 0;
    int start = processBuffer(curState);
    if(curState->rx_buffer[start] == TYPE_VERSION)
    {
        version = curState->rx_buffer[start+6];
    }
    MQTTMsg msg = {.topic = JSON_TOPIC_DEBUG, .type = JSON_PIXY_VERSION, .value = version};
    sendMsgToMQTTQ(msg);
}

void processColor(PIXY_DATA *curState)
{
    static int paused = 0;
    int start = processBuffer(curState);
    if(curState->rx_buffer[start] == TYPE_COLOR)
    {
        curState->blockCount = curState->rx_buffer[start+1]/14;
        if(curState->blockCount > 0)
        {
            if(paused == PIXY_SYNC)
            {
                paused = 0;
                sendMsgToNaviQ(RESUME, 0);
            }
            curState->block.xPos = (curState->rx_buffer[start+7] << 8) | curState->rx_buffer[start+6];
            curState->block.yPos = curState->rx_buffer[start+8];
            panx = (PIXY_X_RANGE/2.0) - curState->block.xPos;
            pany = curState->block.yPos - (PIXY_Y_RANGE/2.0);
            sendMsgToNaviQ(PIXY, curState->block.xPos);
            //sendMsgToPixyQ(PIXY_PAN);
        }
        else
        {
            /*
            sendMsgToNaviQ(PAUSE, 0); //send pause to movement
            paused = PIXY_SYNC;
            panx = 10;
            */
        }
    }
}

void pixy_fsm(PIXY_DATA *curState, uint8_t *type)
{
    dbgOutputLoc(ENTER_PIXY_FSM);
    static uint8_t prevType = 0;
    switch (curState->state)
    {
        case PixyInit:
            curState->block.xPos = 0;
            curState->block.yPos = 0;
            curState->state = PixyWaitingToSend;
            break;
        case PixyWaitingToSend:
            switch(*type)
            {
                case PIXY_VERSION:
                    pixyGetVersion(curState->rx_buffer, curState->tx_buffer);
                    break;
                case PIXY_PAN:
                    pixySetServos(curState->rx_buffer, curState->tx_buffer, panx, pany);
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
                switch(prevType)
                {
                    case PIXY_VERSION:
                        processVersion(curState);
                        break;
                    case PIXY_PAN:
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



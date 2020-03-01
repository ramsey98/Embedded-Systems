/*
 * pixy_state.c
 *
 *  Created on: Feb 26, 2020
 *      Author: Matthew
 */


#include "pixy_state.h"

int pixyFsm(PIXY_DATA *curState, int *timeInc, int *complete, uint64_t *block_data) {
   dbgOutputLoc(SPI_ENTER_FSM);
   int success = 0;
   int i;
   switch (curState->state)
   {
       case PixyInit:
       {
           curState->curTime = 0;
           curState->blockCount = 0;

           memset(curState->blocks, 0, MAX_BLOCKS * sizeof(BLOCK_DATA));

           for(i=0; i < MAX_BLOCKS; i++) {
                curState->blocks[i].colorCode = i+1;
                curState->blocks[i].xPos = 0;
                curState->blocks[i].yPos = 0;
                curState->blocks[i].xPixels = 0;
                curState->blocks[i].yPixels = 0;
                curState->blocks[i].angle = 0;
                curState->blocks[i].trackIndex = 0;
                curState->blocks[i].age = 0;

           }
           curState->state = PixySendVersion;
           break;
       }
       case PixySendVersion:
            if (*timeInc == 0)
            {
            }
            else if (*timeInc > 0)
            {
                dbgOutputLoc(SPI_SEND_VERSION_PACKET);
                curState->curTime += *timeInc;
                spiGetVersionPacket(curState->rx_buffer, curState->tx_buffer);
                curState->state = PixyWaitingForVersion;
                *timeInc = 0;
            }
           break;

       case PixyWaitingForVersion:
           if(*complete > 0) {
               dbgOutputLoc(SPI_RECEIVE_VERSION_PACKET);
               *complete = 0;
               curState->state = PixyWaitingForTime1;
               dbgUARTStr((char *)curState->rx_buffer);
           }
           break;

       case PixyWaitingForTime1:
       {
           if (*timeInc == 0)
           {

           }
           else if (*timeInc > 0)
           {
               dbgOutputLoc(SPI_SEND_CONNECTED_PACKET);
               curState->curTime += *timeInc;
               spiGetConnectedBlocks(curState->rx_buffer, curState->tx_buffer);
               curState->state = PixyWaitingForBlockCount;
               *timeInc = 0;
           }

           break;
       }
       case PixyWaitingForBlockCount:
       {
           if(*complete > 0) {
               dbgOutputLoc(SPI_RECEIVE_CONNECTED_PACKET);
               *complete = 0;
               curState->state = PixyWaitingForBlocks;
               dbgUARTStr("Blocks:");
               dbgUARTVal(curState->rx_buffer[CONNECTED_LENGTH_LOC]);
               curState->blockCount = curState->rx_buffer[CONNECTED_LENGTH_LOC];
               dbgUARTStr("Objects:");
               dbgUARTVal(curState->blockCount/CONNECTED_PACKET_LENGTH);
           }
           break;
       }
       case PixyWaitingForBlocks:
       {
           int loc = CONNECTED_LENGTH_LOC + 2;

           //assign blocks
           for(i = 0; i < curState->blockCount/CONNECTED_PACKET_LENGTH; i++) {
               curState->blocks[i].colorCode = curState->rx_buffer[loc+1];
               curState->blocks[i].xPos = (curState->rx_buffer[loc+2] << 8) | curState->rx_buffer[loc+3];
               curState->blocks[i].yPos = curState->rx_buffer[loc+5];
               curState->blocks[i].xPixels = (curState->rx_buffer[loc+6] << 8) | curState->rx_buffer[loc+7];
               curState->blocks[i].yPos = curState->rx_buffer[loc+9];
               curState->blocks[i].angle = (curState->rx_buffer[loc+6] << 10) | curState->rx_buffer[loc+11];
               curState->blocks[i].trackIndex = curState->rx_buffer[loc+12];
               curState->blocks[i].age = curState->rx_buffer[loc+13];

               loc += CONNECTED_PACKET_LENGTH;
           }

           //dbgUARTStr((char *)curState->rx_buffer);
           curState->state = PixyWaitingForTime1;
           break;
       }
       default:
       {
           success = -1;
           break;
       }
   }

   return success;
}

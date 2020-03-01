/*
 * pixy_state.c
 *
 *  Created on: Feb 26, 2020
 *      Author: Matthew
 */


#include "pixy_state.h"

int pixyFsm(PIXY_DATA *curState, int *timeInc, int *complete, uint64_t *block_data) {
   int success = 0;
   switch (curState->state)
   {
       case PixyInit:
           curState->curTime = 0;
           memset(curState->blocks, 0, MAX_BLOCKS * sizeof(BLOCK_DATA));

           int i;
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
           curState->state = PixyWaitingForTime1;
           break;
       case PixyWaitingForTime1:
           if (*timeInc == 0)
           {

           }
           else if (*timeInc > 0)
           {
               *timeInc = 0;
               curState->curTime += *timeInc;
               spiGetConnectedBlocks(curState->rx_buffer, curState->tx_buffer);
               curState->state = PixyWaitingForBlockCount;
           }

           break;
       case PixyWaitingForBlockCount:
           if(*complete > 0) {
               *complete = 0;
               curState->state = PixyWaitingForBlocks;
               dbgUARTStr("Total Blocks:");
               dbgUARTVal(curState->rx_buffer[18]);
               curState->blockCount = curState->rx_buffer[18]/14;
               dbgUARTStr("# Objects:");
               dbgUARTVal(curState->blockCount);
           }
           break;

       case PixyWaitingForBlocks:
           //dbgUARTStr((char *)curState->rx_buffer);
           curState->state = PixyWaitingForTime1;
           break;
       default:
           success = -1;
           break;
   }

   return success;
}

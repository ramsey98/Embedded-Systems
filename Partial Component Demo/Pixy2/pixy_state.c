/*
 * pixy_state.c
 *
 *  Created on: Feb 26, 2020
 *      Author: Matthew
 */


#include "pixy_state.h"

int pixyFsm(PIXY_DATA *curState, int timeInc, uint64_t *block_data) {
   dbgOutputLoc(ENTER_FSM_SPI);
   int success = 0;
   switch (curState->state)
   {
       case PixyInit:
           dbgOutputLoc(ENTER_FSM_SPI_INIT);
           curState->curTime = 0;

           /*
           memset(curState->blocks, 0, MAX_BLOCKS * sizeof(BLOCK_DATA));

           int i;
           for(i=0; i < MAX_BLOCKS; i++) {
                curState->blocks[i].colorCode = i;
                curState->blocks[i].xPos = 0;
                curState->blocks[i].yPos = 0;
                curState->blocks[i].xPixels = 0;
                curState->blocks[i].yPixels = 0;
                curState->blocks[i].angle = 0;
                curState->blocks[i].trackIndex = 0;
                curState->blocks[i].age = 0;

           } */

           curState->state = PixyWaitingForTime1;
           break;
       case PixyWaitingForTime1:

           dbgOutputLoc(ENTER_FSM_SPI_WAITING);
           if (timeInc == 0)
           {
           }
           else if (timeInc > 0)
           {
               curState->curTime += timeInc;
               spiGetVersionPacket(curState->rx_buffer, curState->tx_buffer, SPI_MSG_LENGTH);
           }

           break;
       case PixyWaitingForBlockCount:
           break;

       case PixyWaitingForBlocks:
           break;
   }

   dbgOutputLoc(EXIT_FSM_SPI);
   return success;
}

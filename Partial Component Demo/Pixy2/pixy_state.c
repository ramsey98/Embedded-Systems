/*
 * pixy_state.c
 *
 *  Created on: Feb 26, 2020
 *      Author: Matthew
 */


#include "pixy_state.h"

void initDistanceData(DISTANCE_DATA *d, PIXY_DATA *p) {
    int j;
    d->blockCount = p->blockCount;
    for(j=0; j < d->blockCount/CONNECTED_PACKET_LENGTH; j++) {
        d->blocks[j] = p->blocks[j];
    }
}

void initBlockData(PIXY_DATA *curState) {
    curState->blockCount = 0;
    memset(curState->blocks, 0, MAX_BLOCKS * sizeof(BLOCK_DATA));

    int i = 0;
    for(i=0; i < MAX_BLOCKS; i++) {
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
    curState->state = PixySendVersion;
}

void assignBlockData(PIXY_DATA *curState) {
    if(curState->blockCount > 0) {

      int loc = CONNECTED_LENGTH_LOC + 2;
      int i;
      //assign blocks
      for(i = 0; i < curState->blockCount/CONNECTED_PACKET_LENGTH; i++) {
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

   curState->state = PixyWaitingForTime1;
}

void checkSendDistanceData(int * sendInc, int * valid_data, PIXY_DATA * curState) {
    if(*sendInc > 0) {
        *sendInc = 0;
        if(*valid_data > 0) {
            DISTANCE_DATA toSend;
            initDistanceData(&toSend, curState);
            sendBlockMsgToDistanceQ1(&toSend);
            *valid_data = 0;
        } else {
            dbgUARTStr("Invalid response from Pixy.\n\r");
        }
    }
}

void checkSendVersionRequest(int * timeInc, PIXY_DATA *curState) {
    if (*timeInc > 0) {
        dbgOutputLoc(SPI_SEND_VERSION_PACKET);
        spiGetVersionPacket(curState->rx_buffer, curState->tx_buffer);
        *timeInc = 0;
        curState->state = PixyWaitingForVersion;
    }
}

void checkSendVersionResponse(int * complete, PIXY_DATA *curState) {
    if(*complete > 0) {
       dbgOutputLoc(SPI_RECEIVE_VERSION_PACKET);    //todo add validity test
       *complete = 0;
       curState->state = PixyWaitingForTime1;
   }
}

void checkSendConnectedBlockRequest(int * timeInc, PIXY_DATA * curState){
    if (*timeInc > 2)
    {
        dbgOutputLoc(SPI_SEND_CONNECTED_PACKET);
        spiGetConnectedBlocks(curState->rx_buffer, curState->tx_buffer);
        curState->state = PixyWaitingForBlockCount;
        *timeInc = 0;
    }
}

void checkSendConnectedBlockResponse(int * complete, int * valid_data, PIXY_DATA * curState) {
    if(*complete > 0) {
        dbgOutputLoc(SPI_RECEIVE_CONNECTED_PACKET);
        *complete = 0;

        if(curState->rx_buffer[CONNECTED_LENGTH_LOC-1] == 33) { //valid data, 1 or more blocks
            curState->blockCount = curState->rx_buffer[CONNECTED_LENGTH_LOC];
            curState->state = PixyWaitingForBlocks;
            *valid_data = 1;

        } else if(curState->rx_buffer[CONNECTED_LENGTH_LOC-2] == 33 && curState->rx_buffer[CONNECTED_LENGTH_LOC-1] == 0) {  //valid data, no blocks
            curState->blockCount = 0;
            curState->state = PixyWaitingForBlocks;
            *valid_data = 1;
        } else {
           curState->state = PixyWaitingForTime1;
        }
    }
}

int pixyFsm(PIXY_DATA *curState, int *timeInc, int *complete, int *sendInc) {
   dbgOutputLoc(SPI_ENTER_FSM);
   int success = 0;
   static int valid_data = 0;
   checkSendDistanceData(sendInc, &valid_data, curState);
   switch (curState->state)
   {
       case PixyInit:
           initBlockData(curState);
           break;
       case PixySendVersion:
           checkSendVersionRequest(timeInc, curState);
           break;
       case PixyWaitingForVersion:
           checkSendVersionResponse(complete, curState);
           break;
       case PixyWaitingForTime1:
           checkSendConnectedBlockRequest(timeInc, curState);
           break;
       case PixyWaitingForBlockCount:
           checkSendConnectedBlockResponse(complete, &valid_data, curState);
           break;
       case PixyWaitingForBlocks:
           assignBlockData(curState);
           break;
       default:
           success = -1;
           break;
   }
   dbgOutputLoc(SPI_EXIT_FSM);
   return success;
}

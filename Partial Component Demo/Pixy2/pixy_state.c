/*
 * pixy_state.c
 *
 *  Created on: Feb 26, 2020
 *      Author: Matthew
 */


#include "pixy_state.h"

int findDistance(BLOCK_DATA *data) {
    int dx;
    int dy;
    if(data->xPixels > (FOCAL_PIXELS_30 + FOCAL_PIXELS_50)/2) {
        dbgUARTVal(30);
        dx = (EGG_WIDTH * FOCAL_LENGTH_30)/FOCAL_PIXELS_30;
    } else if(data->xPixels > (FOCAL_PIXELS_50 + FOCAL_PIXELS_70)/2) {
        dbgUARTVal(50);
        dx = (EGG_WIDTH * FOCAL_LENGTH_50)/FOCAL_PIXELS_50;
    } else if(data->xPixels > (FOCAL_PIXELS_70 + FOCAL_PIXELS_90)/2) {
        dbgUARTVal(70);
        dx = (EGG_WIDTH * FOCAL_LENGTH_70)/FOCAL_PIXELS_70;
    } else {
        dbgUARTVal(90);
        dx = (EGG_WIDTH * FOCAL_LENGTH_90)/FOCAL_PIXELS_90;
    }

    if(data->yPixels > (FOCAL_PIXELS_30_Y + FOCAL_PIXELS_50_Y)/2) {
        dy = (EGG_WIDTH * FOCAL_LENGTH_30_Y)/FOCAL_PIXELS_30_Y;
    } else if(data->yPixels > (FOCAL_PIXELS_50_Y + FOCAL_PIXELS_70_Y)/2) {
        dy = (EGG_WIDTH * FOCAL_LENGTH_50_Y)/FOCAL_PIXELS_50_Y;
    } else if(data->yPixels > (FOCAL_PIXELS_70_Y + FOCAL_PIXELS_90_Y)/2) {
        dy = (EGG_WIDTH * FOCAL_LENGTH_70_Y)/FOCAL_PIXELS_70_Y;
    } else {
        dy = (EGG_WIDTH * FOCAL_LENGTH_90_Y)/FOCAL_PIXELS_90_Y;
    }

    return (0.9*dx + 0.1*dy);


}

int printState(PIXY_DATA *curState) {
    int i;
    for(i = 0; i < curState->blockCount/CONNECTED_PACKET_LENGTH; i++) {
        dbgUARTStr("c:");
        if(curState->blocks[i].colorCode == 1) {
            dbgUARTStr("r");
        } else if(curState->blocks[i].colorCode == 2) {
            dbgUARTStr("g");
        } else if(curState->blocks[i].colorCode == 3) {
            dbgUARTStr("b");
        } else {
            dbgUARTStr("?");
        }
        dbgUARTStr("x:");
        dbgUARTVal(curState->blocks[i].xPos);
        dbgUARTStr("y:");
        dbgUARTVal(curState->blocks[i].yPos);
        dbgUARTStr("xP:");
        dbgUARTVal(curState->blocks[i].xPixels);
        dbgUARTStr("yP:");
        dbgUARTVal(curState->blocks[i].yPixels);
        dbgUARTStr("d:");
        dbgUARTNum(findDistance(&curState->blocks[i]));

        /*
        dbgUARTVal(curState->blocks[i].angle);
        dbgUARTVal(curState->blocks[i].trackIndex);
        dbgUARTVal(curState->blocks[i].age); */
    }

    return 0;
}

int pixyFsm(PIXY_DATA *curState, int *timeInc, int *complete, int *sendInc) {
   dbgOutputLoc(SPI_ENTER_FSM);
   int success = 0;
   int i;
   switch (curState->state)
   {
       case PixyInit:
       {
           curState->curTime = 0;
           curState->blockCount = 0;
           curState->xPan = 0;
           curState->yPan = 0;

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
            if (*timeInc > 0)
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
               curState->state = PixyWaitingForTime1;//PixyPan;
           }
           break;

       /*
       case PixyPan:
           if(*timeInc % 30 == 0 && *timeInc <= 150 && *timeInc > 0) { //every 4.5 seconds

               int pan_pos = *timeInc / 30;
               dbgOutputLoc(SPI_SET_SERVO);
               int panX = (55+ 100*(pan_pos-1));
               dbgUARTNum(panX);
               int panY = 255;
               spiSetServos(curState->rx_buffer, curState->tx_buffer, panX, panY);
           }
           else if(*timeInc == 180) {

           } else if(*timeInc >= 200) {
               curState->state = PixyWaitingForTime1;
               *complete = 0;
               *timeInc = 0;
           }

           break; */

       case PixyWaitingForTime1:
       {
           if(*sendInc > 0) {
               printState(curState);
               curState->curTime += *sendInc;
               *sendInc = 0;
           }
           if (*timeInc > 3)
           {
               dbgOutputLoc(SPI_SEND_CONNECTED_PACKET);

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

               //dbgUARTVal(curState->rx_buffer[CONNECTED_LENGTH_LOC-1]);
               //dbgUARTVal(curState->rx_buffer[CONNECTED_LENGTH_LOC]);
               //dbgUARTVal(curState->rx_buffer[CONNECTED_LENGTH_LOC+1]);

               /*
               int i;
               for(i=15; i < 50; i++) {
                   dbgUARTVal(i);
                   dbgUARTVal(curState->rx_buffer[i]);
               } */
               if(curState->rx_buffer[CONNECTED_LENGTH_LOC-1] != 33) {
                   curState->state = PixyWaitingForTime1;
               } else {
                   curState->blockCount = curState->rx_buffer[CONNECTED_LENGTH_LOC];
                   curState->state = PixyWaitingForBlocks;
               }
           }
           break;
       }
       case PixyWaitingForBlocks:
       {
           dbgUARTStr("Blocks:");
           dbgUARTVal(curState->rx_buffer[CONNECTED_LENGTH_LOC]);
           dbgUARTStr("Objects:");
           dbgUARTVal(curState->blockCount/CONNECTED_PACKET_LENGTH);
           if(curState->blockCount > 0) {

               int loc = CONNECTED_LENGTH_LOC + 2;

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

                   dbgUARTVal(curState->blocks[i].colorCode);
                   dbgUARTVal(curState->blocks[i].xPos);
                   dbgUARTVal(curState->blocks[i].yPos);
                   loc += CONNECTED_PACKET_LENGTH;
               }
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
   dbgOutputLoc(SPI_EXIT_FSM);
   return success;
}

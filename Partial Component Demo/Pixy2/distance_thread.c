/*
 * distance_thread.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#include "distance_thread.h"

void *distanceThread(void *arg0) {

    dbgOutputLoc(ENTER_TASK);
    DISTANCE_DATA data;
    dbgOutputLoc(WHILE1);
    int received = 0;
    int success = 0;
    int transfer = 0;

    while(1) {
        received = recieveMsgFromDistanceQ1(&data, &transfer);
        success = findDistances(&data, &transfer);
        if(success == -1 || received == -1)
        {
            halt();
        }

    }

}

int findDistances(DISTANCE_DATA *data, int * transfer) {
    int success = 0;
    int i;

    if(*transfer > 0){
        *transfer = 0;
        for(i=0; i < data->blockCount/CONNECTED_PACKET_LENGTH; i++) {

            dbgUARTStr("c:");
            if(data->blocks[i].colorCode == 1) {
                dbgUARTStr("r");
            } else if(data->blocks[i].colorCode == 2) {
                dbgUARTStr("g");
            } else if(data->blocks[i].colorCode == 3) {
                dbgUARTStr("y");
            } else {
                dbgUARTStr("?");
            }
            dbgUARTStr("x:");
            dbgUARTVal(data->blocks[i].xPos);
            dbgUARTStr("y:");
            dbgUARTVal(data->blocks[i].yPos);
            dbgUARTStr("xP:");
            dbgUARTVal(data->blocks[i].xPixels);
            dbgUARTStr("yP:");
            dbgUARTVal(data->blocks[i].yPixels);
            dbgUARTStr("d:");
            findDistance(&(data->blocks[i]));
            dbgUARTVal(data->blocks[i].distance);
        }
    }

    return success;
}

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

    data->distance = (0.9*dx + 0.1*dy);
    return data->distance;
}

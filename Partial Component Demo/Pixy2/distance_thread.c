/*
 * distance_thread.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#include "distance_thread.h"

const int focalPixels[FOCAL_LENGTH] = {
                                         36,      //25 cm
                                         30,      //30 cm
                                         28,      //35 cm
                                         24,      //40 cm
                                         22,      //45 cm
                                         20,      //50 cm
                                         20,      //55 cm
                                         16,      //60 cm
                                         16,      //65 cm
                                         14,      //70 cm
                                         14,      //75 cm
                                         14,      //80 cm
                                         12,      //85 cm
                                         12,      //90 cm
                                         12,      //95 cm
                                         12,      //100 cm
                                };

const int focalDistances[FOCAL_LENGTH] = {
                                         25,
                                         30,
                                         35,
                                         40,
                                         45,
                                         50,
                                         55,
                                         60,
                                         65,
                                         70,
                                         75,
                                         80,
                                         85,
                                         90,
                                         95,
                                         100
                                    };

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
            ERROR;
        }

    }

}

int findDistances(DISTANCE_DATA *data, int * transfer) {
    int success = 0;
    int i;
    int first = 1;

    if(*transfer > 0){
        *transfer = 0;
        dbgUARTStr("{ ");
        for(i=0; i < data->blockCount/CONNECTED_PACKET_LENGTH; i++) {

            if(data->blocks[i].xPos > 10) {     //ensuring a proper size

                if(first) {
                    first = 0;
                    dbgUARTStr("{ ");
                } else {
                    dbgUARTStr(" {");
                }
                dbgUARTStr("color:");
                if(data->blocks[i].colorCode == 1) {
                    dbgUARTStr("r ");
                } else if(data->blocks[i].colorCode == 2) {
                    dbgUARTStr("g ");
                } else if(data->blocks[i].colorCode == 3) {
                    dbgUARTStr("y ");
                } else {
                    dbgUARTStr("?");
                }

                findDistance(&(data->blocks[i]));

                /*
                dbgUARTStr("xPos:");
                dbgUARTNumAsChars(data->blocks[i].xPos);
                dbgUARTStr("yPos:");
                dbgUARTNumAsChars(data->blocks[i].yPos);
                dbgUARTStr("xPixels:");
                dbgUARTNumAsChars(data->blocks[i].xPixels);
                dbgUARTStr("yPixels:");
                dbgUARTNumAsChars(data->blocks[i].yPixels);*/
                dbgUARTStr(", distance:");
                dbgUARTNumAsChars(data->blocks[i].distance);

                dbgUARTStr("}");
            }
        }
        dbgUARTStr("}\n\r");
    }

    return success;
}

int findDistance(BLOCK_DATA *data) {
    int dx, i;
    int computed = 0;

    if(data->xPixels > focalPixels[0]) {
        computed = 1;
        int focus = focalPixels[0] * focalDistances[0]/EGG_WIDTH;
        dx = EGG_WIDTH * focus/data->xPixels;
    }

    for(i=1; i < FOCAL_LENGTH-1; i++) {
        if(data->xPixels > (focalPixels[i] + focalPixels[i+1])/2 && data->xPixels < (focalPixels[i] + focalPixels[i-1])/2) {
            computed = 1;
            int focus = focalPixels[i] * focalDistances[i]/EGG_WIDTH;
            dx = EGG_WIDTH * focus/data->xPixels;
        }
    }

    if(computed == 0) {
        int focus = focalPixels[FOCAL_LENGTH-1] * focalDistances[FOCAL_LENGTH-1]/EGG_WIDTH;
        dx = EGG_WIDTH * focus/data->xPixels;
    }

    data->distance = dx;
    return data->distance;
}

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
    int i, closest_object_distance = 1000, closest_index = 0;
    int first = 1;
    MQTTMsg msg = {4, 0, 0, 0, 0};

    if(*transfer > 0){
        *transfer = 0;
        for(i=0; i < data->blockCount/CONNECTED_PACKET_LENGTH; i++) {
            if(data->blocks[i].xPos > 10) {     //ensuring a proper size
                findDistanceAndOffset(&(data->blocks[i]));
                if(data->blocks[i].distance < closest_object_distance) {
                    closest_object_distance = data->blocks[i].distance;
                    closest_index = i;
                }
            }
        }

        if(data->blockCount > 0) { //todo handle this case of no blocks
            //with closest block, send data
            msg.value1 = data->blocks[closest_index].colorCode;
            msg.value2 = data->blocks[closest_index].distance;
            msg.value4 = data->blocks[closest_index].angle;
        }

        sendMsgToMQTTQFromISR(msg);
    }
    return success;
}

void findDistanceAndOffset(BLOCK_DATA *data) {
    int i;
    int computed = 0, focus = 0;

    if(data->xPixels > focalPixels[0]) {
        computed = 1;
        focus = focalPixels[0] * focalDistances[0]/EGG_WIDTH;
    }

    for(i=1; i < FOCAL_LENGTH-1; i++) {
        if(data->xPixels > (focalPixels[i] + focalPixels[i+1])/2 && data->xPixels < (focalPixels[i] + focalPixels[i-1])/2) {
            computed = 1;
            focus = focalPixels[i] * focalDistances[i]/EGG_WIDTH;
        }
    }

    if(computed == 0) {
        focus = focalPixels[FOCAL_LENGTH-1] * focalDistances[FOCAL_LENGTH-1]/EGG_WIDTH;
    }

    data->distance = EGG_WIDTH * focus/data->xPixels;
    int xFromCenterInCm = (157.5 - data->xPos)/(data->xPixels/EGG_WIDTH);
    data->angle = xFromCenterInCm;
}

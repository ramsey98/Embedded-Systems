/*
 * distance_thread.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#include "distance_thread.h"

const int objectFocalPixels[FOCAL_LENGTH] = {
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

const int zumoFocalPixels[FOCAL_LENGTH] = {
                                         78,      //25 cm
                                         76,      //30 cm
                                         70,      //35 cm
                                         64,      //40 cm
                                         56,      //45 cm
                                         52,      //50 cm
                                         48,      //55 cm
                                         43,      //60 cm
                                         40,      //65 cm
                                         38,      //70 cm
                                         36,      //75 cm
                                         30,      //80 cm
                                         28,      //85 cm
                                         24,      //90 cm
                                         22,      //95 cm
                                         20,      //100 cm
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
    MQTTMsg msg = {4, 0, 0, 0, 0};

    if(*transfer > 0){
        *transfer = 0;
        if(data->blockCount == 0) { //send empty message if no blocks
            sendMsgToMQTTQFromISR(msg);
            sendSensorStatePixyInfo(0, 0, 0);
        } else {

            for(i=0; i < data->blockCount/CONNECTED_PACKET_LENGTH; i++) {   //acts almost as an else statement
                if(data->blocks[i].xPos > 10) {             //ensuring a proper size
                    if(data->blocks[i].colorCode < 4) {     //egg
                        findObjectDistanceAndOffset(&(data->blocks[i]));
                    } else {                                //zumo
                        findZumoDistanceAndOffset(&(data->blocks[i]));
                    }
                    if(data->blocks[i].distance < closest_object_distance) {
                        closest_object_distance = data->blocks[i].distance;
                        closest_index = i;
                    }
                    sendMQTTMessageToPixy(&(data->blocks[i]));
                }
            }
            sendSensorStatePixyInfo(data->blocks[closest_index].colorCode, data->blocks[closest_index].distance, data->blocks[closest_index].angle);
        }
    }
    return success;
}

void findObjectDistanceAndOffset(DISTANCE_BLOCK *data) {
    int i;
    int computed = 0, focus = 0;

    if(data->xPixels > objectFocalPixels[0]) {
        computed = 1;
        focus = objectFocalPixels[0] * focalDistances[0]/EGG_WIDTH;
    }

    for(i=1; i < FOCAL_LENGTH-1; i++) {
        if(data->xPixels > (objectFocalPixels[i] + objectFocalPixels[i+1])/2 && data->xPixels < (objectFocalPixels[i] + objectFocalPixels[i-1])/2) {
            computed = 1;
            focus = objectFocalPixels[i] * focalDistances[i]/EGG_WIDTH;
        }
    }

    if(computed == 0) {
        focus = objectFocalPixels[FOCAL_LENGTH-1] * focalDistances[FOCAL_LENGTH-1]/EGG_WIDTH;
    }

    data->distance = EGG_WIDTH * focus/data->xPixels;
    int xFromCenterInCm = (157.5 - data->xPos)/(data->xPixels/EGG_WIDTH);
    data->angle = xFromCenterInCm;
}

void findZumoDistanceAndOffset(DISTANCE_BLOCK *data) {  //todo fill this in.
    int i;
    int computed = 0, focus = 0;

    if(data->xPixels > zumoFocalPixels[0]) {
        computed = 1;
        focus = zumoFocalPixels[0] * focalDistances[0]/ZUMO_WIDTH;
    }

    for(i=1; i < FOCAL_LENGTH-1; i++) {
        if(data->xPixels > (zumoFocalPixels[i] + zumoFocalPixels[i+1])/2 && data->xPixels < (zumoFocalPixels[i] + zumoFocalPixels[i-1])/2) {
            computed = 1;
            focus = zumoFocalPixels[i] * focalDistances[i]/ZUMO_WIDTH;
        }
    }

    if(computed == 0) {
        focus = zumoFocalPixels[FOCAL_LENGTH-1] * focalDistances[FOCAL_LENGTH-1]/ZUMO_WIDTH;
    }

    data->distance = ZUMO_WIDTH * focus/data->xPixels;
    int xFromCenterInCm = (157.5 - data->xPos)/(data->xPixels/ZUMO_WIDTH);
    data->angle = xFromCenterInCm;
}

void sendMQTTMessageToPixy(DISTANCE_BLOCK *data) {
    MQTTMsg msg = {4, 0, 0, 0, 0};
    msg.value1 = data->colorCode;
    msg.value2 = data->distance;
    msg.value4 = data->angle;
    sendMsgToMQTTQFromISR(msg);
}

void sendSensorStatePixyInfo(int color, int distance, int offset) {
    SENSORSTATE_QUEUE_DATA data;
    data.type = pixy2_data;
    data.color = color;
    data.distance = distance;
    data.offset = offset;
    sendSensorStateMsgToQ(&data);
}

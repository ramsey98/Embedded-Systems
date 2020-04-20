/*
 * sensorstate_thread.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Matthew
 */

#include "sensorstate_thread.h"

void *sensorStateThread(void *arg0) {
    dbgOutputLoc(ENTER_TASK);
    SENSORSTATE_QUEUE_DATA sensorStateData, pixyData, sensorData;
    static int newPixy = 0, newSensor=0;

    while(1) {
        receiveFromSensorStateQ(&sensorStateData);

        if(sensorStateData.type == ultrasonic_data) {
            sensorData = sensorStateData;
            newSensor = 1;
        } else if(sensorStateData.type == pixy2_data) {
            pixyData = sensorStateData;
            newPixy = 1;
        }

        if(newPixy == 1 && newSensor == 1) {
            determineSensorState(&sensorData, &pixyData);
            newPixy = 0;
            newSensor = 0;
        }
    }
}

void determineSensorState(SENSORSTATE_QUEUE_DATA * sensorData, SENSORSTATE_QUEUE_DATA * pixyData) {
    MQTTMsg msg = {7, 0, 0, 0, 0};

    msg.value1 = pixyData->color;
    msg.value3 = sensorData->distance;
    msg.value4 = pixyData->offset;

    if(sensorData->distance < 100 && pixyData->color == 0) { //handle obstacle case
        msg.value1 = Obstacle;
    }

    if(pixyData->color == 4 && sensorData->distance > 100) { //for zumo, left and right sometimes misses US distance
        msg.value3 = pixyData->distance;
    }

    if(pixyData->color > 0 && pixyData->color < 5) {    //if egg or zumo, determine if in line
        if(pixyData->offset == 0 || msg.value3/pixyData->offset > 8 || msg.value3/pixyData->offset < -8) {
            msg.value2 = Center;
        } else if(pixyData->offset < 0) {
            msg.value2 = Left;
        } else {
            msg.value2 = Right;
        }
    }

    sendMsgToMQTTQFromISR(msg);
}

/*
 * sensor_thread.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#include "sensor_thread.h"

void *sensorThread(void *arg0) {

    dbgOutputLoc(ENTER_TASK);
    int received = 0, averageTime = 0;
    static int sumSensorTime = 0, numberOfPolls = 0, distance = 0;

    while(1) {
        received = receiveFromSensorQ();

        switch(received) {
            case SENSOR_TYPE_TRIGGER:
                GPIO_write(CONFIG_GPIO_8, CONFIG_GPIO_LED_ON);
                timerTriggerStart();
                numberOfPolls++;
                break;
            case SENSOR_TYPE_SUM:
                averageTime = sumSensorTime/numberOfPolls;
                distance = (CM_PER_US * averageTime)/2 + SENSOR_OFFSET;
                sumSensorTime = 0;
                numberOfPolls = 0;

                //send to queue
                MQTTMsg msg = {5, 0, 0};
                msg.value1 = distance;
                sendMsgToMQTTQFromISR(msg);
                sendSensorStateUSInfo(distance);
                break;

            default:
                sumSensorTime += received;
                break;
        }
    }
}

void sendSensorStateUSInfo(int distance) {
    SENSORSTATE_QUEUE_DATA data;
    data.type = ultrasonic_data;
    data.distance = distance;
    sendSensorStateMsgToQ(&data);
}

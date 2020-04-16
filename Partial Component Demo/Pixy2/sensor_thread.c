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
    static int sumSensorTime = 0;
    static int numberOfPolls = 0;
    static int distance = 0;

    while(1) {
        received = receiveFromSensorQ();

        switch(received) {
            case SENSOR_TYPE_TRIGGER:
                //send 10 us trigger
                GPIO_write(CONFIG_GPIO_8, CONFIG_GPIO_LED_ON);
                timerTriggerStart();
                numberOfPolls++;
                break;
            case SENSOR_TYPE_SUM:
                averageTime = sumSensorTime/numberOfPolls;
                distance = (CM_PER_US * averageTime)/2;
                dbgUARTStr("Sensord: ");
                dbgUARTNumAsChars(distance);
                dbgUARTStr("\n\r");
                sumSensorTime = 0;
                numberOfPolls = 0;
                break;

            default:
                sumSensorTime += received;
                dbgUARTNumAsChars(received);
                break;

        }
    }
}


/*
 * sensor_thread.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#include "sensor_thread.h"

void *sensorThread(void *arg0) {

    dbgOutputLoc(ENTER_TASK);
    int received = 0;

    while(1) {
        received = receiveFromSensorQ();
        dbgUARTStr("Sensor: ");
        dbgUARTNumAsChars(received);
        dbgUARTStr("\n\r");
    }
}

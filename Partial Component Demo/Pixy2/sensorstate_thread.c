/*
 * sensorstate_thread.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Matthew
 */

#include "sensorstate_thread.h"

void *sensorStateThread(void *arg0) {
    dbgOutputLoc(ENTER_TASK);
    SENSORSTATE_QUEUE_DATA sensorStateData;

    while(1) {
        receiveFromSensorStateQ(&sensorStateData);
        //dbgUARTStr("Type: ");
        //dbgUARTNumAsChars(sensorStateData.type);
        //dbgUARTStr("\n\r");
    }
}

/*
 * sensor_queue.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#include "sensor_queue.h"

int sendTimeMsgToQ1(unsigned int timeVal)
{
    q1.timeVals[index] = timeVal;
    index+=1;
    //trigger interrupt
    success = 1;
    return success;
}

int sendSensorMsgToQ1(int mmDist)
{
    q1.mmDists[index] = mmDist;
    index+=1;
    //trigger interrupt
    success = 1;
    return success;
}



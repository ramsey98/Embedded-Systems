/*
 * Main.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */
#include "debug.h"
#include "sensor_state.h"
#include "sensor_queue.h"
#include "timerone.h"
#include "timertwo.h"

void *mainThread(void *arg0)
{
    dbgUARTInit();
    dbgGPIOInit();
    timerOneInit();
    timerTwoInit();
    dbgOutputLoc(ENTER_TASK);
    SENSOR_DATA curState;
    curState.state = Init;
    uint32_t timeInc = 0;
    uint32_t sensorVal = 0;
    createSensorQueue();
    dbgOutputLoc(WHILE1);
    fsm(&curState, timeInc, sensorVal);
    unsigned int received;
    while(1)
    {
        receiveFromQ1((void *) received);
        //determine what is received here
        if(!fsm(&curState, timeInc, sensorVal))
        {
            halt();
        }

    }

}






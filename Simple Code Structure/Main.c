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
    fsm(&curState, timeInc, sensorVal);
    unsigned int received;
    int type;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromQ1((void *) received);
        type = (received & 0xffffffff);
        if (type == 0x00010000)
        {
            timeInc = received & 0x0000ffff;
        }
        else if (type == 0x00020000)
        {
            sensorVal = received & 0x0000ffff;
        }
        if(!fsm(&curState, timeInc, sensorVal))
        {
            halt();
        }

    }

}






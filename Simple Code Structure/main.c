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
    Board_init();
    dbgUARTInit();
    dbgGPIOInit();
    Timer_init();
    timerOneInit();
    timerTwoInit();
    dbgOutputLoc(ENTER_TASK);
    SENSOR_DATA curState;
    curState.state = Init;
    uint32_t timeInc = 0;
    uint32_t sensorVal = 0;
    createSensorQueue();
    fsm(&curState, timeInc, sensorVal);
    unsigned long received;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromQ1((void *) received);
        if (received & 0x00010000 == 0x00010000)
        {
            timeInc = received & 0xffff;
        }
        else if (received & 0x00020000 == 0x00020000)
        {
            sensorVal = received & 0xffff;
        }
        if(fsm(&curState, timeInc, sensorVal) == -1)
        {
            halt();
        }
    }

}






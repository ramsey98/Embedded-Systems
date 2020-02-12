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
    //Board_init();
    dbgUARTInit();
    dbgGPIOInit();
    adcInit();
    createSensorQueue();
    Timer_init();
    timerOneInit();
    timerTwoInit();
    dbgOutputLoc(ENTER_TASK);
    SENSOR_DATA curState;
    curState.state = Init;
    int timeInc = 0;
    int sensorVal = 0;
    int success = fsm(&curState, timeInc, sensorVal);
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromQ1(&timeInc, &sensorVal);
        success = fsm(&curState, timeInc, sensorVal);
        if(success == -1 || received == -1)
        {
            halt();
        }
    }
}






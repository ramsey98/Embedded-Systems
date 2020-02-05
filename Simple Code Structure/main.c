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
    adcInit();
    Timer_init();
    timerOneInit();
    timerTwoInit();
    dbgOutputLoc(ENTER_TASK);
    SENSOR_DATA curState;
    curState.state = Init;
    int timeInc = 0;
    int sensorVal = 0;
    createSensorQueue();
    int success = fsm(&curState, timeInc, sensorVal);
    unsigned long received;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromQ1((void *) received);
        if (received & 0x0000000100000000 == 0x0000000100000000)
        {
            timeInc = received & 0xffffffff;
        }
        else if (received & 0x0000000200000000 == 0x0000000200000000)
        {
            sensorVal = received & 0xffffffff;
        }
        success = fsm(&curState, timeInc, sensorVal);
        if(success == -1)
        {
            halt();
        }
    }

}






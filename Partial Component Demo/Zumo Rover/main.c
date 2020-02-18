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
#include "capture.h"
#include "capture_queue.h"

void *mainThread(void *arg0)
{
    dbgUARTInit();
    dbgGPIOInit();
    adcInit();
    createSensorQueue();
    createCaptureQueue();
    Timer_init();
//    timerOneInit();
//    timerTwoInit();
    Capture_init();
    capture0Init();
    capture1Init();
    dbgOutputLoc(ENTER_TASK);
//    SENSOR_DATA curState;
//    curState.state = Init;
    uint32_t leftFreq = 0;
    uint32_t rightFreq = 0;
    //int success = fsm(&curState, timeInc, sensorVal);
    int received = 0;
    dbgOutputLoc(WHILE1);

    while(1)
    {
        received = receiveFromCapQ(&leftFreq, &rightFreq);
        //success = fsm(&curState, timeInc, sensorVal);
        //if(success == -1 || received == -1)
        if(received == -1)
        {
            halt();
        }
    }
}






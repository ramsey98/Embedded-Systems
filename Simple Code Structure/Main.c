/*
 * Main.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */
#include "debug.h"
#include "sensor_state.h"
#include "sensor_queue.h"
#include "timerone.h"
#include "timertwo.h"

void *mainThread(void *arg0)
{

    curState = Init;
    index = 0;
    dbgUARTInit();
    dbgGPIOInit();
    timerOneInit();
    timerTwoInit();

    dbgOutputLoc(WHILE1);
    while(1) {}

}






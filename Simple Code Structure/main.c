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
    //Power_enablePolicy();
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
    unsigned long received;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromQ1((void *) received);
        if (received & 0x0000000100000000 == 0x0000000100000000)
        {
            timeInc = received & 0xffffffff;
            sensorVal = 0;
        }
        else if (received & 0x0000000200000000 == 0x0000000200000000)
        {
            sensorVal = received & 0xffffffff;
            timeInc = 0;
        }
        success = fsm(&curState, timeInc, sensorVal);

        if(success == -1)
        {
            halt();
        }
        if (curState.state == Init)//WaitingForTime1)
        {
            GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
        }
    }
}






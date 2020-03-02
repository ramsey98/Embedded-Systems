
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
#include "spi.h"
#include "pixy_state.h"
#include "pixy_queue.h"

void *mainThread(void *arg0)
{
    dbgUARTInit();
    dbgGPIOInit();
    adcInit();
    spiInit();
    createSensorQueue();
    createPixyQueue();
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

    int timeIncPixy = 0;
    int complete = 0;
    int count = 0;
    int sendIncPixy = 0;
    PIXY_DATA pixyState;
    pixyState.state = PixyInit;
    initBuffers(pixyState.rx_buffer, pixyState.tx_buffer);
    success = pixyFsm(&pixyState, &timeIncPixy, &complete, &sendIncPixy);

    dbgOutputLoc(WHILE1);
    while(1)
    {
        /*
        received = receiveFromQ1(&timeInc, &sensorVal);
        success = fsm(&curState, timeInc, sensorVal);
        if(success == -1 || received == -1)
        {
            halt();
        } */


        /* */
        received = receiveFromPixyQ1(&timeIncPixy, &complete, &sendIncPixy);
        success = pixyFsm(&pixyState, &timeIncPixy, &complete, &sendIncPixy);
        if(success == -1 || received == -1)
        {
            halt();
        }


        /*
        count++;
        if(count > 1000000) {
            spiGetConnectedBlocks(pixyState.rx_buffer, pixyState.tx_buffer);
            count = 0;

            dbgUARTStr("Blocks:");
            dbgUARTVal(pixyState.rx_buffer[CONNECTED_LENGTH_LOC]);
            pixyState.blockCount = pixyState.rx_buffer[CONNECTED_LENGTH_LOC];
            dbgUARTStr("Objects:");
            dbgUARTVal(pixyState.blockCount/CONNECTED_PACKET_LENGTH);
        } */

    }
}


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

    int count = 0;
    //uint8_t rx_buffer[SPI_MSG_LENGTH];
    //uint8_t tx_buffer[SPI_MSG_LENGTH];
    uint64_t block_data;
    PIXY_DATA pixyState;
    pixyState.state = PixyInit;
    success = pixyFsm(&pixyState, timeInc, &block_data);

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

        received = receiveFromPixyQ1(&timeInc, &block_data);
        success = pixyFsm(&pixyState, timeInc, &block_data);
        if(success == -1 || received == -1)
        {
            halt();
        }

        /*
        count++;

        if(count > 1000000) {
            count = 0;
            spiGetVersionPacket(rx_buffer, tx_buffer, SPI_MSG_LENGTH);
            //spiSetColorPacket(rx_buffer, tx_buffer, SPI_MSG_LENGTH, 0, 255, 0);
        } */
    }
}

/*
 * spi_thread.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#include "spi_thread.h"


void *spiThread(void *arg0) {

    dbgOutputLoc(ENTER_TASK);
    int received = 0;
    int timeIncPixy = 0;
    int complete = 0;
    int sendIncPixy = 0;
    PIXY_DATA pixyState;
    pixyState.state = PixyInit;
    initBuffers(pixyState.rx_buffer, pixyState.tx_buffer);
    int success = pixyFsm(&pixyState, &timeIncPixy, &complete, &sendIncPixy);
    dbgOutputLoc(WHILE1);

    //spiSetServos(pixyState.rx_buffer, pixyState.tx_buffer, 100, 255);

    while(1) {
        received = receiveFromPixyQ1(&timeIncPixy, &complete, &sendIncPixy);
        success = pixyFsm(&pixyState, &timeIncPixy, &complete, &sendIncPixy);
        if(success == -1 || received == -1)
        {
            halt();
        }
    }
}

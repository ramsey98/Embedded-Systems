/*
 * pixy.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "pixy.h"

static SPI_Handle masterSpi;

void pixy_init()
{
    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.bitRate = 10000000;
    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);
    if (masterSpi == NULL)
    {
        ERROR;
    }
}

int pixy_transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t len)
{
    bool transferOK;
    SPI_Transaction transaction;
    transaction.count = len;
    transaction.txBuf = (void *) tx_buffer;
    transaction.rxBuf = (void *) rx_buffer;
    transferOK = SPI_transfer(masterSpi, &transaction);
    return transferOK;
}

void *pixyThread(void *arg0)
{
    pixy_init();
    dbgOutputLoc(ENTER_TASK);
    //int received = 0;
    uint32_t value;
    uint8_t txBuffer[TXBUFFER];
    uint8_t rxBuffer[RXBUFFER];
    uint8_t type = 0x20;
    uint8_t len = 2;
    txBuffer[0] = 0xae;
    txBuffer[1] = 0xc1;
    txBuffer[2] = type;
    txBuffer[3] = len;
    //txBuffer[4] = CCC_SIG_ALL;
    txBuffer[5] = 0xff;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        pixy_transfer(txBuffer, rxBuffer, sizeof(txBuffer));
        value = rxBuffer[4] << 24;
        value = value | (rxBuffer[3] << 16);
        value = value | (rxBuffer[2] << 8);
        value = value | rxBuffer[1];
        /*
        received = receiveFromPixyQ(&value);
        if(received == -1)
        {
            halt();
        }
        else
        {
            memcpy(value, rxBuffer, sizeof(rxBuffer));
            sendMsgToPixyQ(value);
        }
        */
    }
}


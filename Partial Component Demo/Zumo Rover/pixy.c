/*
 * pixy.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "pixy.h"

static SPI_Handle masterSpi;
uint8_t txBuffer[TXBUFFER];
uint8_t rxBuffer[RXBUFFER];

void SPICallback(SPI_Handle handle, SPI_Transaction *trans)
{
    if(trans->status == SPI_TRANSFER_COMPLETED)
    {
        uint32_t value;
        uint8_t type = 0x20;
        uint8_t len = 2;
        txBuffer[0] = 0xae;
        txBuffer[1] = 0xc1;
        txBuffer[2] = type;
        txBuffer[3] = len;
        //txBuffer[4] = CCC_SIG_ALL;
        txBuffer[5] = 0xff;
        value = rxBuffer[4] << 24;
        value = value | (rxBuffer[3] << 16);
        value = value | (rxBuffer[2] << 8);
        value = value | rxBuffer[1];
        sendMsgToPixyQ(value);
    }
    else if(trans->status == SPI_TRANSFER_FAILED)
    {
        ERROR;
    }

}

void pixy_init()
{
    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.transferMode = SPI_MODE_CALLBACK;
    spiParams.bitRate = 2000000;
    spiParams.transferCallbackFxn = SPICallback;
    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);
    if (masterSpi == NULL)
    {
        ERROR;
    }
}

void pixy_transfer()
{
    bool transferOK;
    SPI_Transaction transaction;
    transaction.count = sizeof(txBuffer);
    transaction.txBuf = (void *) txBuffer;
    transaction.rxBuf = (void *) rxBuffer;
    transferOK = SPI_transfer(masterSpi, &transaction);
    if(~transferOK)
    {
        ERROR;
    }
}

void *pixyThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint32_t value;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromPixyQ(&value);
        /*
        if(value == toLeft)
        {
            sendMsgToPIDQ(TURNLEFT, 25);
        }
        else if(value == toRight)
        {
            sendMsgToPIDQ(TURNRIGHT, 25);
        }
        */
    }
}


/*
 * spi.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Matthew
 */
#include "spi.h"

void spiInit()
{
    dbgOutputLoc(SPI_INIT);
    SPI_Params_init(&params);
    //params.bitRate  = someNewBitRate;
    handle = SPI_open(CONFIG_SPI_0, &params);

    if (!handle) {
        halt();
    }
}

void spiTransfer()
{
    dbgOutputLoc(SPI_BEGIN_TRANSFER);
    spiTransaction.count = SPI_MSG_LENGTH;
    spiTransaction.txBuf = (char *) txBuffer;
    spiTransaction.rxBuf = (char *) rxBuffer;

    uint16_t ret = SPI_transfer(handle, &spiTransaction);
    if (!ret) {
        halt();
    } else {
        dbgOutputLoc(SPI_END_TRANSFER);
        dbgUARTStr(rxBuffer);
    }

}

/*
 * spi.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Matthew
 */
#include "spi.h"

unsigned char txBufferInfo[6] = {
                                   0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                   0xc1,  // second byte of no_checksum_sync
                                   0x0e,  // this is the version request type
                                   0x00   // data_length is 2
                                };

void spiCallback(SPI_Handle handle, SPI_Transaction *transaction)
{
   dbgOutputLoc(SPI_CALLBACK);
   if(transaction->status == SPI_TRANSFER_COMPLETED)
   {
       dbgOutputLoc(SPI_SUCCESS_TRANSFER);
       dbgUARTStr((char *) (*transaction).rxBuf);

       //if rx_buffer[2] == 33

       //if code == etc
   }
   else if(transaction->status == SPI_TRANSFER_FAILED) {
       dbgOutputLoc(SPI_FAIL_TRANSFER);
       halt();
   }
   return;
}

void spiInit()
{
    SPI_init();
    dbgOutputLoc(SPI_INIT);
    SPI_Params_init(&params);
    params.transferTimeout = 1000;
    params.bitRate  = 2000000;
    params.transferMode = SPI_MODE_CALLBACK;
    params.frameFormat = SPI_POL0_PHA1;
    params.transferCallbackFxn = spiCallback;
    params.dataSize = 8;
    handle = SPI_open(CONFIG_SPI_0, &params);

    int i;
    for(i=0; i < SPI_TX_LENGTH; i++) {
        if(i < 4) {
            txBuffer[i] = txBufferInfo[i];
        } else {
            txBuffer[i] = 0xff;
        }
    }

    for(i=0; i < SPI_RX_LENGTH; i++) {
        rxBuffer[i] = 0;
    }

    if (!handle) {
        halt();
    }
}

void spiTransfer()
{
    dbgOutputLoc(SPI_BEGIN_TRANSFER);
    spiTransaction.count = SPI_TX_LENGTH;
    spiTransaction.txBuf = (void *) txBuffer;
    spiTransaction.rxBuf = (void *) rxBuffer;
    SPI_transfer(handle, &spiTransaction);
}

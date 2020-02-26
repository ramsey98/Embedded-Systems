/*
 * spi.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Matthew
 */
#include "spi.h"

void spiCallback(SPI_Handle handle, SPI_Transaction *transaction)
{
   dbgOutputLoc(SPI_CALLBACK);
   if(transaction->status == SPI_TRANSFER_COMPLETED)
   {
       dbgOutputLoc(SPI_SUCCESS_TRANSFER);
       uint8_t *rx_buffptr = (*transaction).rxBuf;

       dbgUARTVal(rx_buffptr[0]);
       dbgUARTVal(rx_buffptr[3]);

       //color connected components response
       if(rx_buffptr[2] == 33) {
           uint8_t packet_length = rx_buffptr[3];
           uint8_t block_count = packet_length/CONNECTED_PACKET_LENGTH;
           dbgUARTVal(block_count);
       }

       //get version response
       else if(rx_buffptr[2] == 15) {

       }
   } else {
       //dbgOutputLoc(SPI_FAIL_TRANSFER);
   }

   return;
}

void spiInit()
{
    SPI_init();
    dbgOutputLoc(SPI_INIT);
    SPI_Params_init(&params);
    params.bitRate  = 2000000;
    params.transferMode = SPI_MODE_CALLBACK;
    params.frameFormat = SPI_POL0_PHA1;
    params.transferCallbackFxn = spiCallback;
    handle = SPI_open(CONFIG_SPI_0, &params);

    if (!handle) {
        halt();
    }
}

void setTxBuffer(uint8_t *tx_buffer, uint8_t *tx_msg, unsigned tx_length, unsigned msg_length) {
    int i;
    for(i=0; i < tx_length; i++) {
        if(i < msg_length) {
            tx_buffer[i] = tx_msg[i];
        } else {
            tx_buffer[i] = 0xff;
        }
    }
}

void setRxBuffer(uint8_t *rx_buffer, unsigned rx_length) {
    memset((void *) rx_buffer, 0, rx_length);
}

void spiGetVersionPacket(uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frame_count) {
    uint8_t txMsgVersion[SPI_TX_MSG_VERSION] = {
                                                     0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                     0xc1,  // second byte of no_checksum_sync
                                                     0x0e,  // this is the version request type
                                                     0x00  // data_length is 0
                                               };
    setRxBuffer(rx_buffer, frame_count);
    setTxBuffer(tx_buffer, txMsgVersion, frame_count, SPI_TX_MSG_VERSION);

    spiTransfer(frame_count, rx_buffer, tx_buffer);
}

void spiSetColorPacket(uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frame_count, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t txMsgColor[SPI_TX_MSG_COLOR] = {
                                                     0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                     0xc1,  // second byte of no_checksum_sync
                                                     0x14,  // this is the version request type
                                                     0x03,  // data_length is 0
                                                     r,
                                                     g,
                                                     b
                                               };
    setRxBuffer(rx_buffer, frame_count);
    setTxBuffer(tx_buffer, txMsgColor, frame_count, SPI_TX_MSG_COLOR);

    spiTransfer(frame_count, rx_buffer, tx_buffer);
}


void spiGetConnectedBlocks(uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frame_count) {
    uint8_t txMsgConnected[SPI_TX_MSG_CONNECTED] = {
                                                       0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                       0xc1,  // second byte of no_checksum_sync
                                                       0x20,  // this is the version request type
                                                       0x02,   // data_length is 2
                                                       0xff,
                                                       0xff,
                                                    };

    setRxBuffer(rx_buffer, frame_count);
    setTxBuffer(tx_buffer, txMsgConnected, frame_count, SPI_TX_MSG_CONNECTED);

    spiTransfer(frame_count, rx_buffer, tx_buffer);
}

void spiTransfer(unsigned frame_count, uint8_t *rx_buffer, uint8_t *tx_buffer)
{
    dbgOutputLoc(SPI_BEGIN_TRANSFER);
    SPI_Transaction spi_transaction;
    spi_transaction.count = frame_count;
    spi_transaction.txBuf = (void *) tx_buffer;
    spi_transaction.rxBuf = (void *) rx_buffer;
    SPI_transfer(handle, &spi_transaction);
    dbgOutputLoc(SPI_WAIT_CALLBACK);
}

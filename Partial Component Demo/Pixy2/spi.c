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
       sendSpiMsgToPixyQ1();
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
            tx_buffer[i] = 0x00;
        }
    }
}

void initBuffers(uint8_t *rx_buffer, uint8_t *tx_buffer) {
    memset(rx_buffer, 0, SPI_MSG_LENGTH);
    memset(tx_buffer, 0, SPI_MSG_LENGTH);
}

void spiGetVersionPacket(uint8_t *rx_buffer, uint8_t *tx_buffer) {
    uint8_t txMsgVersion[SPI_TX_MSG_VERSION] = {
                                                     0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                     0xc1,  // second byte of no_checksum_sync
                                                     0x0e,  // this is the version request type
                                                     0x00  // data_length is 0
                                               };
    memset(rx_buffer, 0, SPI_MSG_LENGTH);
    setTxBuffer(tx_buffer, txMsgVersion, SPI_MSG_LENGTH, SPI_TX_MSG_VERSION);
    spiTransfer(rx_buffer, tx_buffer);
}

void spiSetServos(uint8_t *rx_buffer, uint8_t *tx_buffer, int panX, int panY) {

    uint8_t msg4 = (uint8_t)(panX >> 8);
    uint8_t msg5 = (uint8_t)(panX & 0xff);
    uint8_t msg6 = (uint8_t)(panY >> 8);
    uint8_t msg7 = (uint8_t)(panY & 0xff);

    uint8_t txMsgServos[SPI_TX_MSG_SERVOS] = {
                                                     0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                     0xc1,  // second byte of no_checksum_sync
                                                     0x12,  // this is the version request type
                                                     0x04,  // data_length is 0
                                                     msg4,
                                                     msg5,
                                                     msg6,
                                                     msg7

                                               };
    memset(rx_buffer, 0, SPI_MSG_LENGTH);
    setTxBuffer(tx_buffer, txMsgServos, SPI_MSG_LENGTH, SPI_TX_MSG_SERVOS);
    spiTransfer(rx_buffer, tx_buffer);
}

void spiSetColorPacket(uint8_t *rx_buffer, uint8_t *tx_buffer, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t txMsgColor[SPI_TX_MSG_COLOR] = {
                                                     0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                     0xc1,  // second byte of no_checksum_sync
                                                     0x14,  // this is the version request type
                                                     0x03,  // data_length is 0
                                                     r,
                                                     g,
                                                     b
                                               };
    memset(rx_buffer, 0, SPI_MSG_LENGTH);
    setTxBuffer(tx_buffer, txMsgColor, SPI_MSG_LENGTH, SPI_TX_MSG_COLOR);
    spiTransfer(rx_buffer, tx_buffer);
}


void spiGetConnectedBlocks(uint8_t *rx_buffer, uint8_t *tx_buffer) {
    uint8_t txMsgConnected[SPI_TX_MSG_CONNECTED] = {
                                                       0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                       0xc1,  // second byte of no_checksum_sync
                                                       0x20,  // this is the version request type
                                                       0x02,   // data_length is 2
                                                       0xff,
                                                       0xff,
                                                    };
    //memset(rx_buffer, 0, SPI_MSG_LENGTH);
    setTxBuffer(tx_buffer, txMsgConnected, SPI_MSG_LENGTH, SPI_TX_MSG_CONNECTED);
    spiTransfer(rx_buffer, tx_buffer);
}

void spiTransfer(uint8_t *rx_buffer, uint8_t *tx_buffer)
{
    dbgOutputLoc(SPI_BEGIN_TRANSFER);
    spi_transaction.count = SPI_MSG_LENGTH;
    spi_transaction.txBuf = (void *) tx_buffer;
    spi_transaction.rxBuf = (void *) rx_buffer;
    SPI_transfer(handle, &spi_transaction);
    dbgOutputLoc(SPI_WAIT_CALLBACK);
}

/*
 * pixy.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "pixy.h"
#include "debug_queue.h"

static SPI_Handle masterSpi;

void SPICallback(SPI_Handle handle, SPI_Transaction *trans)
{
    if(trans->status == SPI_TRANSFER_COMPLETED)
    {
        sendMsgToPixyQFromISR(PIXY_COMPLETE);
    }
    else if(trans->status == SPI_TRANSFER_FAILED) ERROR;
}

void pixyInit()
{
    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.bitRate = SPI_RATE;
    //spiParams.transferCallbackFxn = SPICallback;
    masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);
    if (masterSpi == NULL) ERROR;
}

void pixy_transfer(uint8_t *rx_buffer, uint8_t *tx_buffer)
{
    SPI_Transaction transaction;
    transaction.count = SPI_MSG_LENGTH;
    transaction.txBuf = (void *) tx_buffer;
    transaction.rxBuf = (void *) rx_buffer;
    if(SPI_transfer(masterSpi, &transaction) == 0) ERROR;
    sendMsgToPixyQ(PIXY_COMPLETE);
}

void *pixyThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint8_t type = 0;
    PIXY_DATA pixyState;
    pixyState.state = PixyInit;
    initBuffers(pixyState.rx_buffer, pixyState.tx_buffer);
    pixy_fsm(&pixyState, &type);
    sendMsgToPixyQ(PIXY_PAN);
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromPixyQ(&type);
        pixy_fsm(&pixyState, &type);
    }
}


void setTxBuffer(uint8_t *tx_buffer, uint8_t *tx_msg, unsigned tx_length, unsigned msg_length) {
    int i;
    for(i=0; i < tx_length; i++)
    {
        if(i < msg_length)
        {
            tx_buffer[i] = tx_msg[i];
        }
        else
        {
            tx_buffer[i] = 0x00;
        }
    }
}

void initBuffers(uint8_t *rx_buffer, uint8_t *tx_buffer)
{
    memset(rx_buffer, 0, SPI_MSG_LENGTH);
    memset(tx_buffer, 0, SPI_MSG_LENGTH);
}

void pixyGetVersion(uint8_t *rx_buffer, uint8_t *tx_buffer)
{
    uint8_t txMsgVersion[SPI_TX_MSG_VERSION] = {
                                                     0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                     0xc1,  // second byte of no_checksum_sync
                                                     0x0e,  // this is the version request type
                                                     0x00  // data_length is 0
                                               };
    memset(rx_buffer, 0, SPI_MSG_LENGTH);
    setTxBuffer(tx_buffer, txMsgVersion, SPI_MSG_LENGTH, SPI_TX_MSG_VERSION);
    pixy_transfer(rx_buffer, tx_buffer);
}

void pixySetServos(uint8_t *rx_buffer, uint8_t *tx_buffer, uint16_t panX, uint16_t panY)
{
    uint8_t msg4 = panX;
    uint8_t msg5 = panX >> 8;
    uint8_t msg6 = panY;
    uint8_t msg7 = panY >> 8;
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
    pixy_transfer(rx_buffer, tx_buffer);
}

void pixySetColor(uint8_t *rx_buffer, uint8_t *tx_buffer, uint8_t r, uint8_t g, uint8_t b)
{
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
    pixy_transfer(rx_buffer, tx_buffer);
}


void pixyGetConnectedBlocks(uint8_t *rx_buffer, uint8_t *tx_buffer)
{
    uint8_t txMsgConnected[SPI_TX_MSG_CONNECTED] = {
                                                       0xae,  // first byte of no_checksum_sync (little endian -> least-significant byte first)
                                                       0xc1,  // second byte of no_checksum_sync
                                                       0x20,  // this is the version request type
                                                       0x02,   // data_length is 2
                                                       0xff,
                                                       0xff,
                                                    };
    memset(rx_buffer, 0, SPI_MSG_LENGTH);
    setTxBuffer(tx_buffer, txMsgConnected, SPI_MSG_LENGTH, SPI_TX_MSG_CONNECTED);
    pixy_transfer(rx_buffer, tx_buffer);
}


/*
 * spi.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Matthew
 */

#ifndef SPI_H_
#define SPI_H_

#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "FreeRTOS.h"
#include "debug.h"
#include "string.h"

#define SPI_MSG_LENGTH 128
#define SPI_TX_MSG_VERSION 4
#define SPI_TX_MSG_CONNECTED 6
#define SPI_TX_MSG_COLOR 7
#define CONNECTED_PACKET_LENGTH 14

void spiInit();
void spiCallback(SPI_Handle handle, SPI_Transaction *transaction);
void setTxBuffer(uint8_t *tx_buffer, uint8_t *tx_msg, unsigned tx_length, unsigned msg_length);
void setRxBuffer(uint8_t *rx_buffer, unsigned rx_length);
void spiGetVersionPacket(uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frame_count);
void spiSetColorPacket(uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frame_count, uint8_t r, uint8_t g, uint8_t b);
void spiGetConnectedBlocks(uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frame_count);
void spiTransfer(unsigned frame_count, uint8_t *rx_buffer, uint8_t *tx_buffer);

SPI_Handle      handle;
SPI_Params      params;

#endif /* SPI_H_ */

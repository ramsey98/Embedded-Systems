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
#include "pixy_queue.h"

#define SPI_MSG_LENGTH 128
#define SPI_TX_MSG_VERSION 4
#define SPI_TX_MSG_CONNECTED 6
#define SPI_TX_MSG_COLOR 7
#define SPI_TX_MSG_SERVOS 8
#define CONNECTED_PACKET_LENGTH 14
#define CONNECTED_LENGTH_LOC 18
#define FOCAL_PIXELS_30 30
#define FOCAL_PIXELS_50 20
#define FOCAL_PIXELS_70 16
#define FOCAL_PIXELS_90 14
#define FOCAL_LENGTH_30 195.725
#define FOCAL_LENGTH_50 224.972
#define FOCAL_LENGTH_70 251.969
#define FOCAL_LENGTH_90 283.464
#define FOCAL_PIXELS_30_Y 34.5
#define FOCAL_PIXELS_50_Y 32
#define FOCAL_PIXELS_70_Y 19
#define FOCAL_PIXELS_90_Y 17
#define FOCAL_LENGTH_30_Y 232.845
#define FOCAL_LENGTH_50_Y 359.95
#define FOCAL_LENGTH_70_Y 299.21
#define FOCAL_LENGTH_90_Y 344.206
#define EGG_WIDTH 4.445

void spiInit();
void spiCallback(SPI_Handle handle, SPI_Transaction *transaction);
void setTxBuffer(uint8_t *tx_buffer, uint8_t *tx_msg, unsigned tx_length, unsigned msg_length);
void initBuffers(uint8_t *rx_buffer, uint8_t *tx_buffer);
void spiGetVersionPacket(uint8_t *rx_buffer, uint8_t *tx_buffer);
void spiSetServos(uint8_t *rx_buffer, uint8_t *tx_buffer, int panX, int panY);
void spiSetColorPacket(uint8_t *rx_buffer, uint8_t *tx_buffer, uint8_t r, uint8_t g, uint8_t b);
void spiGetConnectedBlocks(uint8_t *rx_buffer, uint8_t *tx_buffer);
void spiTransfer(uint8_t *rx_buffer, uint8_t *tx_buffer);

SPI_Handle      handle;
SPI_Params      params;
SPI_Transaction spi_transaction;

#endif /* SPI_H_ */

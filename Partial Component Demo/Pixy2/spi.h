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

#define SPI_RX_LENGTH 128
#define SPI_TX_LENGTH 6

void spiInit();
void spiTransfer();

SPI_Handle      handle;
SPI_Params      params;
SPI_Transaction spiTransaction;

unsigned char rxBuffer[SPI_RX_LENGTH];
unsigned char txBuffer[SPI_TX_LENGTH];

#endif /* SPI_H_ */

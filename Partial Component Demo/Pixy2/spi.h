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

void spiCallback(SPI_Handle handle, SPI_Transaction *transaction);
void spiInit();
void spiTransfer();

SPI_Handle      handle;
SPI_Params      params;
SPI_Transaction spiTransaction;

uint8_t rxBuffer[SPI_MSG_LENGTH];
uint8_t txBuffer[SPI_MSG_LENGTH];

#endif /* SPI_H_ */

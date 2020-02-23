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

#define SPI_MSG_LENGTH 128

void spiInit();
void spiTransfer();

SPI_Handle      handle;
SPI_Params      params;
SPI_Transaction spiTransaction;

char  rxBuffer[SPI_MSG_LENGTH];
char  txBuffer[6] = {0b11111111, 0b11111111, 0b00000010, 0b00100000, 0b11000001, 0b10101110};

#endif /* SPI_H_ */

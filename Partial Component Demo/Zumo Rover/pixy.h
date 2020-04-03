/*
 * pixy.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef PIXY_H_
#define PIXY_H_

#include <ti/drivers/SPI.h>
#include "debug.h"
#include "pixy_queue.h"
#include "pixy_state.h"
#include "PID_queue.h"

void pixy_init();
void SPICallback(SPI_Handle handle, SPI_Transaction *trans);
void setTxBuffer(uint8_t *tx_buffer, uint8_t *tx_msg, unsigned tx_length, unsigned msg_length);
void initBuffers(uint8_t *rx_buffer, uint8_t *tx_buffer);
void pixy_transfer(uint8_t *rx_buffer, uint8_t *tx_buffer);
void *pixyThread(void *arg0);

#endif /* PIXY_H_ */

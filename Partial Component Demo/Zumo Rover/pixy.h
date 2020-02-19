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

#define TXBUFFER (8)
#define RXBUFFER (32)

void pixy_init();
int pixy_transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, uint8_t len);
void *pixyThread(void *arg0);


#endif /* PIXY_H_ */

/*
 * spi_thread.h
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#ifndef SPI_THREAD_H_
#define SPI_THREAD_H_

#include "debug.h"
#include "spi.h"
#include "pixy_queue.h"
#include "pixy_state.h"

void *spiThread(void *arg0);

#endif /* SPI_THREAD_H_ */

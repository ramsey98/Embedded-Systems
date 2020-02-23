/*
 * UARTTx_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef UARTTX_QUEUE_H_
#define UARTTX_QUEUE_H_

#include "debug.h"
#include "queue.h"

//https://www.pololu.com/docs/0J25/5
#define INIT_CONTROLLER (0xAA)
#define GET_FIRMWARE (0x81)
#define GET_ERROR (0x82)
//bits 0-2 unused
//bit 3: Data Overrun Error
//bit 4: Frame Error
//bit 5: CRC Error
//bit 6: Format Error
//bit 7: Timeout
#define GET_DEVICE_ID (0x0083)
#define GET_PWM_PARAM (0x0183)
#define GET_SHUTDOWN_ON_ERROR (0x0283)
#define GET_SERIAL_TIMEOUT (0x0383)
#define M0_COAST (0x86)
#define M1_COAST (0x87)
//M0 Commands
#define M0_FORWARD (0x88) //followed by motor speed
#define M0_FORWARD_8BIT (0x89) //speed + 128 (max is 0x7f) 127+128
#define M0_REVERSE (0x8A) //motor speed
#define M0_REVERSE_8BIT (0x8B) //speed+128
//M1 Commands
#define M1_FORWARD (0x8C) //followed by motor speed
#define M1_FORWARD_8BIT (0x8D) //speed + 128 (max is 0x7f) 127+128
#define M1_REVERSE (0x8E) //motor speed
#define M1_REVERSE_8BIT (0x8F) //speed+128

int sendMsgToUARTTxQ(uint16_t value);
int receiveFromUARTTxQ(uint16_t * value);
void createUARTTxQueue();

#endif /* UARTTX_QUEUE_H_ */
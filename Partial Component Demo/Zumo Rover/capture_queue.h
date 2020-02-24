/*
 * capture_queue.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef CAPTURE_QUEUE_H_
#define CAPTURE_QUEUE_H_

#include "debug.h"
#include "queue.h"

#define CAPFMASK (0xFF)
#define LEFT (0x01)
#define RIGHT (0x02)
#define ENCODERSHIFT (8)

int sendLeftMsgToCapQ(uint8_t freq);
int sendRightMsgToCapQ(uint8_t freq);
int receiveFromCapQ(uint8_t * leftFreq, uint8_t * rightFreq);
void createCaptureQueue();

#endif /* CAPTURE_QUEUE_H_ */

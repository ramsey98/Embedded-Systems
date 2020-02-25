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

#define CAPFMASK (0xFFFFFFFF)
#define LEFT (0x00000001)
#define RIGHT (0x00000002)
#define ENCODERSHIFT (32)

int sendLeftMsgToCapQ(uint32_t period);
int sendRightMsgToCapQ(uint32_t period);
int receiveFromCapQ(uint32_t * type, uint32_t * period);
void createCaptureQueue();

#endif /* CAPTURE_QUEUE_H_ */

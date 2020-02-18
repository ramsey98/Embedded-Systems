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
#define LEFTMASK (0x00000001)
#define RIGHTMASK (0x00000002)
#define LEFTFLAG (0x0000000100000000)
#define RIGHTFLAG (0x0000000200000000)
#define ENCODERSHIFT (32)

int sendLeftMsgToCapQ(uint32_t period);
int sendRightMsgToCapQ(uint32_t period);
int receiveFromCapQ(uint32_t * leftFreq, uint32_t * rightFreq);
void createCaptureQueue();

#endif /* CAPTURE_QUEUE_H_ */

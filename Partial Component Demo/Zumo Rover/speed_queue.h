/*
 * speed_queue.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef SPEED_QUEUE_H_
#define SPEED_QUEUE_H_

#include "debug.h"
#include "queue.h"

#define SPEEDFMASK (0xFFFF)
#define LEFTSPEEDMASK (0x01)
#define RIGHTSPEEDMASK (0x02)
#define ACCELMASK (0x03)
#define DECELMASK (0x04)
#define PAUSEMASK (0x05)
#define RESUMEMASK (0x06)
#define LEFTSPEEDFLAG (0x0100)
#define RIGHTSPEEDFLAG (0x0200)
#define ACCELFLAG (0x0300)
#define DECELFLAG (0x0400)
#define PAUSEFLAG (0x0500)
#define RESUMEFLAG (0x0600)
#define SPEEDSHIFT (8)

int sendLeftMsgToSpeedQ(uint8_t value);
int sendRightMsgToSpeedQ(uint8_t value);
int sendAccelMsgToSpeedQ(uint8_t value);
int sendDecelMsgToSpeedQ(uint8_t value);
int sendPauseMsgToSpeedQ();
int sendResumeMsgToSpeedQ();
int receiveFromSpeedQ(uint16_t * value);
void createSpeedQueue();

#endif /* SPEED_QUEUE_H_ */

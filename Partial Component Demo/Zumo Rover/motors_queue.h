/*
 * motors_queue.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef MOTORS_QUEUE_H_
#define MOTORS_QUEUE_H_

#include "debug.h"
#include "queue.h"

#define MOTORSFMASK (0xFFFF)
#define LEFTMOTORSMASK (0x01)
#define RIGHTMOTORSMASK (0x02)
#define ACCELMOTORSMASK (0x03)
#define DECELMOTORSMASK (0x04)
#define PAUSEMOTORSMASK (0x05)
#define RESUMEMOTORSMASK (0x06)
#define LEFTMOTORSFLAG (0x0100)
#define RIGHTMOTORSFLAG (0x0200)
#define ACCELMOTORSFLAG (0x0300)
#define DECELMOTORSFLAG (0x0400)
#define PAUSEMOTORSFLAG (0x0500)
#define RESUMEMOTORSFLAG (0x0600)
#define MOTORSSHIFT (8)

int sendLeftMsgToMotorsQ(uint8_t value);
int sendRightMsgToMotorsQ(uint8_t value);
int sendAccelMsgToMotorsQ(uint8_t value);
int sendDecelMsgToMotorsQ(uint8_t value);
int sendPauseMsgToMotorsQ();
int sendResumeMsgToMotorsQ();
int receiveFromMotorsQ(uint8_t * type, uint8_t * value);
void createMotorsQueue();

#endif /* MOTORS_QUEUE_H_ */

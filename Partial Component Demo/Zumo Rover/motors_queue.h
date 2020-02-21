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
#define LEFTFORWARDMOTORSMASK (0x01)
#define LEFTREVERSEMOTORSMASK (0x02)
#define RIGHTFORWARDMOTORSMASK (0x03)
#define RIGHTREVERSEMOTORSMASK (0x04)
#define ACCELMOTORSMASK (0x05)
#define DECELMOTORSMASK (0x06)
#define PAUSEMOTORSMASK (0x07)
#define RESUMEMOTORSMASK (0x08)

#define LEFTFORWARDMOTORSFLAG (0x0100)
#define LEFTREVERSEMOTORSFLAG (0x0200)
#define RIGHTFORWARDMOTORSFLAG (0x0300)
#define RIGHTREVERSEMOTORSFLAG (0x0400)
#define ACCELMOTORSFLAG (0x0500)
#define DECELMOTORSFLAG (0x0600)
#define PAUSEMOTORSFLAG (0x0700)
#define RESUMEMOTORSFLAG (0x0800)
#define MOTORSSHIFT (8)

int sendLeftForwardMsgToMotorsQ(uint8_t value);
int sendLeftReverseMsgToMotorsQ(uint8_t value);
int sendRightForwardMsgToMotorsQ(uint8_t value);
int sendRightReverseMsgToMotorsQ(uint8_t value);
int sendAccelMsgToMotorsQ(uint8_t value);
int sendDecelMsgToMotorsQ(uint8_t value);
int sendPauseMsgToMotorsQ();
int sendForwardMsgToMotorsQ();
int sendPauseMsgToMotorsQ();
int sendResumeMsgToMotorsQ();
int receiveFromMotorsQ(uint8_t * type, uint8_t * value);
void createMotorsQueue();

#endif /* MOTORS_QUEUE_H_ */

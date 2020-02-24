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

#define MOTORSFMASK (0xFF)
#define LEFTFORWARD (0x01)
#define LEFTREVERSE (0x02)
#define RIGHTFORWARD (0x03)
#define RIGHTREVERSE (0x04)
#define ACCEL (0x05)
#define DECEL (0x06)
#define PAUSE (0x07)
#define RESUME (0x08)
#define MOTORSSHIFT (8)

int sendLeftForwardMsgToMotorsQ(uint8_t value);
int sendLeftReverseMsgToMotorsQ(uint8_t value);
int sendRightForwardMsgToMotorsQ(uint8_t value);
int sendRightReverseMsgToMotorsQ(uint8_t value);
int sendAccelMsgToMotorsQ(uint8_t value);
int sendDecelMsgToMotorsQ(uint8_t value);
int sendPauseMsgToMotorsQ();
int sendPauseMsgToMotorsQ();
int sendResumeMsgToMotorsQ();
int receiveFromMotorsQ(uint8_t * type, uint8_t * value);
void createMotorsQueue();

#endif /* MOTORS_QUEUE_H_ */

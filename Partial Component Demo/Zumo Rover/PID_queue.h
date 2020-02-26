/*
 * motors_queue.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef PID_QUEUE_H_
#define PID_QUEUE_H_

#include "debug.h"
#include "UARTDebug_queue.h"
#include "queue.h"

#define MOTORSFMASK (0xFF)
#define LEFTCAP (0x00000001)
#define RIGHTCAP (0x00000002)
#define TIMER (0x00000003)
#define TURNLEFT (0x00000004)
#define TURNRIGHT (0x00000005)
#define REVERSE (0x00000006)
#define FORWARD (0x00000007)
#define ACCEL (0x00000008)
#define DECEL (0x00000009)
#define PAUSE (0x0000000A)
#define RESUME (0x0000000B)
#define LEFTCOUNT (0x0000000C)
#define RIGHTCOUNT (0x0000000D)

#define PIDSHIFT (32)

void sendMsgToPIDQ(uint32_t type, uint32_t value);
void receiveFromPIDQ(uint32_t * type, uint32_t * value);
void createPIDQueue();

#endif /* PID_QUEUE_H_ */

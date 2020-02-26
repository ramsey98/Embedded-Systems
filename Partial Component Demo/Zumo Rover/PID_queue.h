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
#define SENSOR (0x00000004)
#define PIXY (0x00000005)
#define TURNLEFT (0x00000006)
#define TURNRIGHT (0x00000007)
#define REVERSE (0x00000008)
#define FORWARD (0x00000009)
#define ACCEL (0x0000000A)
#define DECEL (0x0000000B)
#define PAUSE (0x0000000C)
#define RESUME (0x0000000D)
#define LEFTCOUNT (0x0000000E)
#define RIGHTCOUNT (0x0000000F)

#define PIDSHIFT (32)

void sendMsgToPIDQ(uint32_t type, uint32_t value);
void receiveFromPIDQ(uint32_t * type, uint32_t * value);
void createPIDQueue();

#endif /* PID_QUEUE_H_ */

/*
 * motors_queue.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef PID_QUEUE_H_
#define PID_QUEUE_H_

#include <debug_queue.h>
#include "debug.h"
#include "queue.h"

#define LEFTCAP 1
#define RIGHTCAP 2
#define TIMER 3
#define SENSOR 4
#define PIXY 5
#define TURNLEFT 6
#define TURNRIGHT 7
#define REVERSE 8
#define FORWARD 9
#define ACCEL 10
#define DECEL 11
#define PAUSE 12
#define RESUME 13
#define PID_SENSOR 14
#define SYNCING 15
#define TIMER_PID 16

#define PIDSHIFT (32)

void sendMsgToPIDQFromISR(uint32_t type, uint32_t value);
void sendMsgToPIDQ(uint32_t type, uint32_t value);
void receiveFromPIDQ(uint32_t * type, uint32_t * value);
void createPIDQueue();

#endif /* PID_QUEUE_H_ */

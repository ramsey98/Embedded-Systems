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
#define LEFTCOUNT 14
#define RIGHTCOUNT 15

#define PIDSHIFT (32)

void sendMsgToPIDQ(uint32_t type, uint32_t value);
void receiveFromPIDQ(uint32_t * type, uint32_t * value);
void createPIDQueue();

#endif /* PID_QUEUE_H_ */

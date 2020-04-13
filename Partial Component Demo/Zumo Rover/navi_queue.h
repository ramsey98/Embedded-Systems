/*
 * motors_queue.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef NAVI_QUEUE_H_
#define NAVI_QUEUE_H_

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
#define NAVI_SENSOR 14
#define PIXY_SYNC 15
#define PID_KP 16
#define PID_KI 17
#define PIXY_X 18
#define PIXY_Y 19

#define PIXY_X_RANGE 315
#define PIXY_Y_RANGE 207

#define NAVISHIFT (32)

void sendMsgToNaviQFromISR(uint32_t type, uint32_t value);
void sendMsgToNaviQ(uint32_t type, uint32_t value);
void receiveFromNaviQ(uint32_t * type, uint32_t * value);
void createNaviQueue();

#endif /* NAVI_QUEUE_H_ */

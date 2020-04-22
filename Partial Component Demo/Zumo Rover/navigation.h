/*
 * navi.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <debug_queue.h>
#include <motors.h>
#include <navi_queue.h>
#include "capture.h"
#include "debug.h"
#include "mqtt_queue.h"
#include "json_parse.h"

#define KP 1.0
#define KI 0.1
#define NAVILOOKUPLEN 13

typedef struct
{
    uint8_t expected;
    uint32_t measured;
} naviLookupTable;

typedef struct
{
    uint8_t motorID;
    uint8_t setSpeed, direction, adjustedSpeed;
    uint8_t forward8Bit, forward, reverse8Bit, reverse;
    uint32_t measuredSpeed;
} MOTOR_DATA;

typedef struct
{
    uint8_t paused;
    MOTOR_DATA leftMotor, rightMotor;
} MOTORS_STATE;

void PIDAdjust(MOTOR_DATA *motor);
void motorAccel(MOTOR_DATA *motor, uint8_t value);
void motorDecel(MOTOR_DATA *motor, uint8_t value);
void motorSet(MOTOR_DATA *motor, uint8_t value);
void updateMotors(MOTORS_STATE motorsState, MOTOR_DATA motor);
void updateValues(MOTORS_STATE * motorsState, uint32_t type, uint32_t value);
void *naviThread(void *arg0);

#endif /* NAVIGATION_H_ */

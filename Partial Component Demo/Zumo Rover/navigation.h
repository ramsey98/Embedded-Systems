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

#define KP 0.5
#define KI 0.5
#define NAVILOOKUPLEN 14

typedef struct
{
    uint8_t expected;
    uint32_t measured;
} naviLookupTable;

typedef struct
{
    uint8_t setLeftSpeed, setRightSpeed, leftDir, rightDir, paused;//, distance, distLeft, distRight;
    uint32_t realLeftSpeed, realRightSpeed;
} MOTORS_DATA;

int PIDAdjust(uint8_t setSpeed, uint8_t measuredSpeed);
void updateMotors(MOTORS_DATA motorsState);
void updateValues(MOTORS_DATA * motorsState, uint32_t type, uint32_t value);
void *naviThread(void *arg0);

#endif /* NAVIGATION_H_ */

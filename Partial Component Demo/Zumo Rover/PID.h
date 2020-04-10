/*
 * PID.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef PID_H_
#define PID_H_

#include <debug_queue.h>
#include <motors.h>
#include "capture.h"
#include "debug.h"
#include <PID_queue.h>

#define PIDLOOKUPLEN 13

typedef struct
{
    uint8_t expected;
    uint32_t measured;
} PIDLookupTable;

typedef struct
{
    uint8_t setLeftSpeed, setRightSpeed, leftDir, rightDir, paused;//, distance, distLeft, distRight;
    uint32_t realLeftSpeed, realRightSpeed;
} MOTORS_DATA;

void PIDAdjust(MOTORS_DATA *motorsState);
void updateMotors(MOTORS_DATA motorsState);
void updateValues(MOTORS_DATA * motorsState, uint32_t type, uint32_t value);
void *PIDThread(void *arg0);

#endif /* PID_H_ */

/*
 * capture_state.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Holden Ramsey
 */

#ifndef CAPTURE_STATE_H_
#define CAPTURE_STATE_H_

#include "debug.h"
#include "UARTDebug_queue.h"
#include "capture_queue.h"

#define LINECOUNT (12)
#define MINUTE (60)
#define SECOND (1000000)

typedef enum
{
    Capture_Init,
    Capture_Averaging,
    Capture_Polling,
} CAPTURE_STATES;

typedef struct
{
    CAPTURE_STATES state;
    uint32_t leftTotal, rightTotal, leftAvg, rightAvg;
    int count, leftCount, rightCount;
} CAPTURE_DATA;

int capture_fsm(CAPTURE_DATA *curState, uint32_t type, uint32_t period);

#endif /* CAPTURE_STATE_H_ */

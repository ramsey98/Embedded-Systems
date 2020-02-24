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

typedef enum
{
    Capture_Init,
    Capture_Averaging,
    Capture_Polling,
} CAPTURE_STATES;

typedef struct
{
    CAPTURE_STATES state;
    uint32_t leftTotal, rightTotal;
    int count, leftCount, rightCount;
    uint8_t leftAvg, rightAvg;
} CAPTURE_DATA;

int capture_fsm(CAPTURE_DATA *curState, uint8_t type, uint8_t freq);

#endif /* CAPTURE_STATE_H_ */

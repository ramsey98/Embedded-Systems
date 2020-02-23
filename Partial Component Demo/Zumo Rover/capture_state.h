/*
 * capture_state.h
 *
 *  Created on: Feb 23, 2020
 *      Author: Holden Ramsey
 */

#ifndef CAPTURE_STATE_H_
#define CAPTURE_STATE_H_

#include "debug.h"

typedef enum
{
    Capture_Init,
    Capture_Waiting,
    Capture_Polling,
} CAPTURE_STATES;

typedef struct
{
    CAPTURE_STATES state;
    uint8_t leftFreq, rightFreq;
    int count;
} CAPTURE_DATA;

int capture_fsm(CAPTURE_DATA *curState, uint8_t leftFreq, uint8_t rightFreq);

#endif /* CAPTURE_STATE_H_ */

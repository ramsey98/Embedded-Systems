/*
 * motors_state.h
 *
 *  Created on: Feb 21, 2020
 *      Author: Holden Ramsey
 */

#ifndef PID_STATE_H_
#define PID_STATE_H_

#include <PID_queue.h>
#include "debug.h"
#include "UARTTx_queue.h"

typedef enum
{
    Motors_Init,
    Motors_Ready,
    Motors_Paused,
} MOTORS_STATES;

typedef struct
{
    MOTORS_STATES state;
    uint8_t leftSpeed, rightSpeed, leftDir, rightDir;

} MOTORS_DATA;

int motors_fsm(MOTORS_DATA *motorsState, uint32_t type, uint32_t value);

#endif /* PID_STATE_H_ */

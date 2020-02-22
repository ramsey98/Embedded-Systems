/*
 * motors_state.h
 *
 *  Created on: Feb 21, 2020
 *      Author: Holden Ramsey
 */

#ifndef MOTORS_STATE_H_
#define MOTORS_STATE_H_

#include "debug.h"
#include "UARTTx_queue.h"
#include "motors_queue.h"

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

int motors_fsm(MOTORS_DATA *motorsState, uint8_t type, uint8_t value);

#endif /* MOTORS_STATE_H_ */

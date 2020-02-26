/*
 * motors.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef PID_H_
#define PID_H_


#include "capture.h"
#include "debug.h"
#include <PID_queue.h>
#include "UARTTx_queue.h"
#include "UARTDebug_queue.h"

typedef struct
{
    uint8_t leftSpeed, rightSpeed, leftDir, rightDir;
    int paused;
} MOTORS_DATA;

void motorsUARTInit();
void updateValues(MOTORS_DATA * motorsState, uint32_t type, uint32_t value);
void *PIDThread(void *arg0);
void *UARTTxThread(void *arg0);

#endif /* PID_H_ */

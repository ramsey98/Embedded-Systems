/*
 * motors.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef PID_H_
#define PID_H_

#include <PID_queue.h>
#include <PID_state.h>
#include "capture.h"
#include "UARTDebug_queue.h"
#include "debug.h"
#include "UARTTx_queue.h"

void motorsUARTInit();
void *PIDThread(void *arg0);
void *UARTTxThread(void *arg0);

#endif /* PID_H_ */

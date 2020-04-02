/*
 * capture.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#include "debug.h"
#include "UARTDebug_queue.h"
#include "PID_queue.h"
//#include "mqtt_queue.h"
#include <ti/drivers/Capture.h>

void captureCallback(Capture_Handle handle, uint32_t interval);
void captureInit();
void clearCounts();
int getLeftCount();
int getRightCount();

#endif /* CAPTURE_H_ */

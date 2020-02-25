/*
 * capture.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#include "debug.h"
#include "capture_state.h"
#include "capture_queue.h"
#include "mqtt_queue.h"
#include <ti/drivers/Capture.h>
#include <ti/drivers/dpl/SemaphoreP.h>

void *captureThread(void *arg0);
void capture0Callback(Capture_Handle handle, uint32_t interval);
void capture0Init();
void capture1Callback(Capture_Handle handle, uint32_t interval);
void capture1Init();

#endif /* CAPTURE_H_ */

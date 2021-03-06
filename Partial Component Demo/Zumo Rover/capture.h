/*
 * capture.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#include <debug_queue.h>
#include <navi_queue.h>
#include "debug.h"
#include <ti/drivers/Capture.h>

void captureCallback(Capture_Handle handle, uint32_t interval);
void captureInit();

#endif /* CAPTURE_H_ */

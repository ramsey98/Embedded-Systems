/*
 * mqtt_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef DEBUG_QUEUE_H_
#define DEBUG_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "debug.h"

void sendMsgToDebugQ(int msg);
void receiveFromDebugQ(int *received);
void createDebugQueue();

#endif /* DEBUG_QUEUE_H_ */

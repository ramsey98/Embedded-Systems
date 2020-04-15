/*
 * config.h
 *
 *  Created on: Apr 5, 2020
 *      Author: Holden Ramsey
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "debug.h"
#include "queue.h"
#include "mqtt_queue.h"

#define CONFIG_STATE 1
#define CONFIG_PID_KP 2
#define CONFIG_PID_KI 3
#define SET_SPEED 4
#define ROVER_MOVING (1)
#define ROVER_LOADING (2)

void * configThread(void *arg0);
void sendMsgToConfigQFromISR(MQTTMsg msg);
void receiveFromConfigQ(MQTTMsg *received);
void createConfigQueue();

#endif /* CONFIG_H_ */

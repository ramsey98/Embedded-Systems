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
#define CONFIG_PID_ENABLE 2
#define CONFIG_SET_SPEED 3
#define CONFIG_TURN_RIGHT 4
#define CONFIG_TURN_LEFT 5
#define CONFIG_SENSOR_ENABLE 6
#define CONFIG_PIXY_ENABLE 7
#define ROVER_MOVING (1)
#define ROVER_LOADING (2)

void * configThread(void *arg0);
void sendMsgToConfigQFromISR(MQTTMsg msg);
void receiveFromConfigQ(MQTTMsg *received);
void createConfigQueue();

#endif /* CONFIG_H_ */

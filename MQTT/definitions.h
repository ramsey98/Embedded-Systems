/*
 * definitions.h
 *
 *  Created on: Mar 11, 2020
 *      Author: Holden Ramsey
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <ti/utils/json/json.h>
#include "debug.h"

#define MQTT_ROVER_STATE (1)

#define SUBSCRIPTION_TOPIC "/team20/zumo"
#define SUBSCRIPTION_TOPIC_COUNT 1

#define PUBLISH_TOPIC "/team20/zumo_stats"
#define PUBLISH_TOPIC_COUNT 1
#define JSON_LEN (2)
#define JSON_FORMAT      \
"{"                        \
    "\"ID\": int32,"        \
    "\"state\": char,"    \
"}"

typedef struct
{
    uint8_t type;
    uint8_t value;
} MQTTMsg;
void initValues();
void parseValues(Json_Handle objectHandle);

#endif /* DEFINITIONS_H_ */

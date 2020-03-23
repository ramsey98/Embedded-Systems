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

#define SUBSCRIPTION_TOPIC "/team20/debug"
#define SUBSCRIPTION_TOPIC_COUNT 1
#define JSON_SUB      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": char"    \
"}"
#define JSON_SUB_BUF      \
"{"                        \
    "\"ID\": 0,"        \
    "\"item1\": 0"    \
"}"


#define PUBLISH_TOPIC_0 "/team20/stats"
#define PUBLISH_TOPIC_1 "/team20/debug"
#define PUBLISH_TOPIC_COUNT 2
#define JSON_PUB      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": char,"    \
"}"

#define JSON_PUB_BUF      \
"{"                        \
    "\"ID\": 0,"        \
    "\"item1\": 0"    \
"}"

typedef struct
{
    uint8_t type;
    uint8_t value;
} MQTTMsg;

void parseValues(Json_Handle objectHandle);
void sendValues(Json_Handle templateHandle, MQTTMsg msg, int msgID);

#endif /* DEFINITIONS_H_ */

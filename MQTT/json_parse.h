/*
 * json_parse.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include <ti/utils/json/json.h>
#include "mqtt_queue.h"
#include "debug.h"

#define JSON_DATA_BUFFER_SIZE (512)
#define JSON_TOPIC_BUFFER_SIZE (256)
#define JSON_ERROR_FORMAT (1)
#define JSON_ERROR_OVERFLOW (2)
#define JSON_ERROR_MISSED_ID (3)
#define JSON_ERROR_REPEAT_ID (4)
#define JSON_TYPE_STATS (1)
#define JSON_TYPE_DEBUG (2)
#define JSON_TYPE_ERROR (3)

#define PUBLISH_TOPIC_0 "/team20/stats"
#define PUBLISH_TOPIC_1 "/team20/debug"
#define PUBLISH_TOPIC_2 "/team20/errors"
#define PUBLISH_TOPIC_COUNT 3

#define JSON_STATS      \
"{"                        \
    "\"ID\": int32,"        \
    "\"Attempts\": int32,"    \
    "\"Received\": int32,"\
    "\"Missed\": int32"\
"}"

#define JSON_STATS_BUF      \
"{"                        \
    "\"ID\": 0,"        \
    "\"Attempts\": 0,"    \
    "\"Received\": 0,"\
    "\"Missed\": 0"\
"}"

#define JSON_DEBUG      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": int32,"    \
"}"
#define JSON_DEBUG_BUF      \
"{"                        \
    "\"ID\": 0,"        \
    "\"item1\": 0"    \
"}"

#define JSON_ERRORS      \
"{"                        \
    "\"ID\": int32,"        \
    "\"Type\": int32"    \
"}"

#define JSON_ERRORS_BUF      \
"{"                        \
    "\"ID\": 0,"        \
    "\"Type\": 0"    \
"}"

#define SUBSCRIPTION_TOPIC "/team20/config"
#define SUBSCRIPTION_TOPIC_COUNT 1
#define JSON_CONFIG      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": int32"    \
"}"
#define JSON_CONFIG_BUF      \
"{"                        \
    "\"ID\": 0,"        \
    "\"item1\": 0"    \
"}"



void json_miss();
void json_receive(char *payload, char *msgTopic);
void json_read_config(Json_Handle objectHandle);
void json_send_stats(Json_Handle objectHandle);
void json_send_debug(MQTTMsg msg, Json_Handle objectHandle);
void json_send_error(MQTTMsg msg, Json_Handle objectHandle);
void json_send(char *publish_topic, char *publish_data, MQTTMsg msg);

#endif /* JSON_PARSE_H_ */

/*
 * json_parse.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include <ti/utils/json/json.h>
#include "definitions.h"
#include "mqtt_queue.h"
#include "debug.h"

#define JSON_DATA_BUFFER_SIZE (512)
#define JSON_TOPIC_BUFFER_SIZE (256)

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

void json_miss();
void json_receive(char *payload, char *msgTopic);
void json_read(char *payload, int *msgID);
void json_send_stats(char *payload);
void json_send_debug(char *payload, MQTTMsg msg);
void json_send(char *publish_topic, char *publish_data, MQTTMsg msg);

#endif /* JSON_PARSE_H_ */

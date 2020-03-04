/*
 * json_parse.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include <ti/utils/json/json.h>
//#include "mqtt_queue.h"

#define PUBLISH_JSON_BUFFER_SIZE (512)

#define JSON_STATS      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": int32,"    \
    "\"item2\": int32,"\
    "\"item3\": int32"\
"}"

#define JSON_ROVER      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": int32,"    \
    "\"item2\": int32,"\
    "\"item3\": int32"\
"}"

#define JSON_SENSORS      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": int32,"    \
    "\"item2\": int32,"\
    "\"item3\": int32"\
"}"

#define JSON_ARM      \
"{"                        \
    "\"ID\": int32,"        \
    "\"item1\": int32,"    \
    "\"item2\": int32,"\
    "\"item3\": int32"\
"}"

#define JSON_ZUMO      \
"{"                        \
    "\"ID\": int32,"        \
    "\"state\": char,"    \
    "\"leftmotor\": char,"\
    "\"rightmotor\": char"\
"}"

typedef enum
{
    JSON_Stats,
    JSON_Rover,
    JSON_Sensors,
    JSON_Arm,
    JSON_Zumo,
} MSG_TYPES;

void json_receive(char *payload, MSG_TYPES msgType);
void json_read(char *payload, char *msgType, const char *keys[4], int *msgID, int *item1, int *item2, int *item3);
void json_send(char *payload, MSG_TYPES msgType, int item1, int item2, int item3);
void json_write(char *payload, char *msgType, const char *keys[4], int msgID, int item1, int item2, int item3);

#endif /* JSON_PARSE_H_ */

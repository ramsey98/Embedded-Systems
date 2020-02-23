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

#define PUBLISH_JSON_BUFFER_SIZE (512)

#define JSON_RECEIVE      \
"{"                        \
    "\"ID\": char,"        \
    "\"state\": char,"    \
"}"

#define JSON_SEND      \
"{"                        \
    "\"ID\": char,"        \
    "\"state\": char,"    \
    "\"leftmotor\": char,"\
    "\"rightmotor\": char"\
"}"

int json_read(char *payload, uint8_t *msgType, uint8_t *state);
int json_write(char *payload, uint8_t msgType, uint8_t state, uint8_t leftmotor, uint8_t rightmotor);

#endif /* JSON_PARSE_H_ */

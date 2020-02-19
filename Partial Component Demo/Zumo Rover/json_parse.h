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

#define JSON_TEMPLATE      \
"{"                        \
    "\"ID\": char,"        \
    "\"state\": int32,"    \
    "\"leftmotor\": int32,"\
    "\"rightmotor\": int32"\
"}"

int json_read(char *payload, uint8_t *msgType, uint8_t *state, uint16_t *leftmotor, uint16_t *rightmotor);
int json_write(char *payload, uint8_t msgType, uint8_t state, uint16_t leftmotor, uint16_t rightmotor);

#endif /* JSON_PARSE_H_ */

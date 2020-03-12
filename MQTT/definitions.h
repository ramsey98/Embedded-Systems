/*
 * definitions.h
 *
 *  Created on: Mar 11, 2020
 *      Author: Holden Ramsey
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <ti/utils/json/json.h>

#define SUBSCRIPTION "/team20/zumo"
#define JSON_LEN (2)
#define JSON_FORMAT      \
"{"                        \
    "\"ID\": int32,"        \
    "\"state\": char,"    \
"}"
void parseValues(Json_Handle objectHandle);

#endif /* DEFINITIONS_H_ */

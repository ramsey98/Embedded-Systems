/*
 * test.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#ifndef TEST_H_
#define TEST_H_

#include <PID.h>
#include "sensor.h"
#include "pixy.h"
#include "mqtt_client.h"
#include "debug.h"
#include <unistd.h>

typedef enum
{
    test0,
    test1,
    test2,
    test3,
    test4,
    test5,
    test6,
    test7,
    test8,
    test9,
    test10,
    end,
} test_num;

void *testThread(void *arg0);
void motortests(test_num* num);
void pixytests(test_num* num);
void sensortests(test_num* num);
void pidtests(test_num* num);
void mqtttests(test_num* num);

#endif /* TEST_H_ */

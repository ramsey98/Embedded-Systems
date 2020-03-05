/*
 * tests.h
 *
 *  Created on: Feb 26, 2020
 *      Author: jared
 */

#ifndef TEST_H_
#define TEST_H_

#include "debug.h"
#include "motor_controller_thread.h"
#include "motor_encoder_thread.h"
//#include "mqtt_queue.h"
#include <unistd.h>
#include <stdio.h>

typedef enum
{
    init,
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
    test11,
    test12,
    end,
} test_num;

void *testThread(void *arg0);
void tests(test_num* num);

#endif /* TEST_H_ */

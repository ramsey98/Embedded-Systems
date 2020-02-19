/*
 * test.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "test.h"

void *testThread(void *arg0)
{
    test_num num = test0;
    while(1)
    {
        tests(&num);
    }
}

void tests(test_num* num)
{
    switch (*num)
    {
    case test0:
        //Interface with controller
        sendMsgToUARTTxQ(INIT_CONTROLLER);
        sendMsgToUARTTxQ(GET_FIRMWARE);
        sendMsgToUARTTxQ(GET_CONFIG);
        sendMsgToUARTTxQ(DEVICE_ID);
        sendMsgToUARTTxQ(GET_CONFIG);
        sendMsgToUARTTxQ(PWM_PARAM);
        sendMsgToUARTTxQ(GET_CONFIG);
        sendMsgToUARTTxQ(SHUTDOWN_ON_ERROR);
        sendMsgToUARTTxQ(GET_CONFIG);
        sendMsgToUARTTxQ(SERIAL_TIMEOUT);
        *num = test1;
        break;
    case test1:
        //left motor forward max speed
        sendMsgToUARTTxQ(M0_FORWARD);
        sendMsgToUARTTxQ(0xFF);
        sendMsgToUARTTxQ(M1_FORWARD);
        sendMsgToUARTTxQ(0x00);
        *num = test2;
        break;
    case test2:
        //right motor forward max speed
        sendMsgToUARTTxQ(M0_FORWARD);
        sendMsgToUARTTxQ(0x00);
        sendMsgToUARTTxQ(M1_FORWARD);
        sendMsgToUARTTxQ(0xFF);
        *num = test3;
        break;
    case test3:
        //stop moving
        sendMsgToUARTTxQ(M0_FORWARD);
        sendMsgToUARTTxQ(0x00);
        sendMsgToUARTTxQ(M1_REVERSE);
        sendMsgToUARTTxQ(0x00);
        *num = test4;
        break;
    case test4:
        //both motors forward max speed
        sendMsgToUARTTxQ(M0_FORWARD);
        sendMsgToUARTTxQ(0xFF);
        sendMsgToUARTTxQ(M1_FORWARD);
        sendMsgToUARTTxQ(0xFF);
        *num = test5;
        break;
    case test5:
        //both motors reverse max speed
        sendMsgToUARTTxQ(M0_REVERSE);
        sendMsgToUARTTxQ(0xFF);
        sendMsgToUARTTxQ(M1_REVERSE);
        sendMsgToUARTTxQ(0xFF);
        *num = test6;
        break;
    case test6:
        //both motors forward max speed
        sendMsgToUARTTxQ(M0_FORWARD);
        sendMsgToUARTTxQ(0x00);
        sendMsgToUARTTxQ(M1_REVERSE);
        sendMsgToUARTTxQ(0x00);
        *num = test7;
        break;
    case test7:
        //turn right
        sendMsgToUARTTxQ(M0_FORWARD);
        sendMsgToUARTTxQ(0xFF);
        sendMsgToUARTTxQ(M1_REVERSE);
        sendMsgToUARTTxQ(0x7F);
        *num = test8;
        break;
    case test8:
        //turn left
        sendMsgToUARTTxQ(M0_FORWARD);
        sendMsgToUARTTxQ(0x7F);
        sendMsgToUARTTxQ(M1_REVERSE);
        sendMsgToUARTTxQ(0xFF);
        *num = test9;
        break;
    case test9:
        *num = test10;
        break;
    case test10:
        *num = end;
        break;
    case end:
        break;
    default:
        break;
    }
    sleep(5);
}


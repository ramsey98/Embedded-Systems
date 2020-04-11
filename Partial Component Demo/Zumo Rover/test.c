/*
 * test.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "test.h"

void *testThread(void *arg0)
{
    //test_num num = test1;
    while(1)
    {
        //pixytests(&num);
        //motortests(&num);
        capturetests();
    }
}


void capturetests()
{
    int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 127};
    int i;
    for(i = 0; i < 13; i++)
    {
        GPIO_toggle(CONFIG_LED_0_GPIO);
        sendMsgToPIDQ(FORWARD, values[i]);
        sleep(10);
    }
}

void pixytests(test_num* num)
{
    switch (*num)
    {
        case test1:
            //sendMsgToPixyQ(PIXY_VERSION);
            //sleep(1);
            //sendMsgToPixyQ(PIXY_PAN);
            //sleep(1);
            //sendMsgToPixyQ(PIXY_COLOR);
            //sleep(1);
            *num = test2;
            break;
        case test2:
            break;
        default:
            break;
    }
}

void sensortests(test_num* num)
{
    switch (*num)
    {
        case test1:
            *num = test2;
            break;
        default:
            break;
    }
}

void pidtests(test_num* num)
{
    switch (*num)
    {
        case test1:
            *num = test2;
            break;
        default:
            break;
    }
}

void mqtttests(test_num* num)
{
    switch (*num)
    {
        case test1:
            *num = test2;
            break;
        default:
            break;
    }
}

void motortests(test_num* num)
{
    switch (*num)
    {
    case test1:
        //Interface with controller
        sendMsgToUARTTxQ(GET_FIRMWARE, EMPTY);
        sendMsgToUARTTxQ(DEVICE_ID, GET_CONFIG);
        sendMsgToUARTTxQ(PWM_PARAM, GET_CONFIG);
        sendMsgToUARTTxQ(SHUTDOWN_ON_ERROR, GET_CONFIG);
        sendMsgToUARTTxQ(SERIAL_TIMEOUT, GET_CONFIG);
        *num = test2;
        break;
    case test2:
        //left motor forward max speed
        sendMsgToUARTTxQ(0x7F, M0_FORWARD);
        sendMsgToUARTTxQ(0x00, M1_FORWARD);
        sleep(3);
        sendMsgToUARTTxQ(0x00, M0_FORWARD);
        sendMsgToUARTTxQ(0x00, M1_FORWARD);
        *num = test3;
        break;
    case test3:
        //right motor forward max speed
        sendMsgToUARTTxQ(0x00, M0_FORWARD);
        sendMsgToUARTTxQ(0x7F, M1_FORWARD);
        sleep(3);
        sendMsgToUARTTxQ(0x00, M0_FORWARD);
        sendMsgToUARTTxQ(0x00, M1_FORWARD);
        *num = test4;
        break;
    case test4:
        //both motors forward max speed
        sendMsgToPIDQ(FORWARD, 0x7F);
        sleep(3);
        sendMsgToPIDQ(FORWARD, 0x00);
        *num = test5;
        break;
    case test5:
        //both motors reverse max speed
        sendMsgToPIDQ(REVERSE, 0x7F);
        sleep(3);
        sendMsgToPIDQ(REVERSE, 0x00);
        *num = test6;
        break;
    case test6:
        //both motors forward then backward half speed
        sendMsgToPIDQ(FORWARD, 0x3F);
        sleep(3);
        sendMsgToPIDQ(REVERSE, 0x3F);
        sleep(3);
        sendMsgToPIDQ(REVERSE, 0x00);
        *num = test7;
        break;
    case test7:
        //turn right then left
        sendMsgToPIDQ(TURNRIGHT, 0x7F);
        sleep(3);
        sendMsgToPIDQ(TURNLEFT, 0x7F);
        sleep(3);
        sendMsgToPIDQ(FORWARD, 0x00);
        *num = test8;
        break;
    case test8:
        //Accel then Decel
        sendMsgToPIDQ(FORWARD, 0x00);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sendMsgToPIDQ(FORWARD, 0);
        *num = test9;
        break;
    case test9:
        //accelerate mid right turn
        sendMsgToPIDQ(TURNRIGHT, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(ACCEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sleep(1);
        sendMsgToPIDQ(DECEL, 25);
        sendMsgToPIDQ(FORWARD, 0);
        *num = test10;
        break;
    case test10:
        //Pause & Resume
        sendMsgToPIDQ(FORWARD, 0x3F);
        sleep(3);
        sendMsgToPIDQ(PAUSE, 0);
        sleep(3);
        sendMsgToPIDQ(RESUME, 0);
        sleep(3);
        sendMsgToPIDQ(FORWARD, 0);
        *num = end;
        break;
    case end:
        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
        break;
    default:
        break;
    }
    sleep(5);
    GPIO_toggle(CONFIG_LED_0_GPIO);
}


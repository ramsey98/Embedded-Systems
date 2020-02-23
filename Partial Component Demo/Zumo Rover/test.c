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
        sendMsgToUARTTxQ(GET_FIRMWARE);
        sendMsgToUARTTxQ(GET_DEVICE_ID);
        sendMsgToUARTTxQ(GET_PWM_PARAM);
        sendMsgToUARTTxQ(GET_SHUTDOWN_ON_ERROR);
        sendMsgToUARTTxQ(GET_SERIAL_TIMEOUT);
        *num = test1;
        break;
    case test1:
        //left motor forward half speed
        sendMsgToUARTTxQ(0x7F88);
        sendMsgToUARTTxQ(0x008C);
        *num = test2;
        break;
    case test2:
        //right motor forward half speed
        sendMsgToUARTTxQ(0x0088);
        sendMsgToUARTTxQ(0x7F8C);
        *num = test3;
        break;
    case test3:
        //both motors forward half speed
        sendMsgToUARTTxQ(0x7F88);
        sendMsgToUARTTxQ(0x7F8C);
        *num = test4;
        break;
    case test4:
        //both motors forward max speed
        sendMsgToUARTTxQ(0x7F89);
        sendMsgToUARTTxQ(0x7F8D);
        *num = test5;
        break;
    case test5:
        //both motors reverse half speed
        sendMsgToUARTTxQ(0x7F8A);
        sendMsgToUARTTxQ(0x7F8E);
        *num = test6;
        break;
    case test6:
        //both motors reverse max speed
        sendMsgToUARTTxQ(0x7F8B);
        sendMsgToUARTTxQ(0x7F8F);
        *num = test7;
        break;
    case test7:
        //turn right
        sendMsgToUARTTxQ(0x7F89);
        sendMsgToUARTTxQ(0x7F8E);
        *num = test8;
        break;
    case test8:
        //turn left
        sendMsgToUARTTxQ(0x7F8A);
        sendMsgToUARTTxQ(0x7F8D);
        *num = test9;
        break;
    case test9:
        sendMsgToUARTTxQ(0x008A);
        sendMsgToUARTTxQ(0x008D);
        *num = test10;
        break;
    case test10:
        //sendLeftMotorMsgToMQTTQ(50);
        //sendRightMotorMsgToMQTTQ(70);
        //sendStateMsgToMQTTQ(40);
        *num = end;
        break;
    case end:
        /*
        if(error_gpio == high)
        {
            sendMsgToUARTTxQ(GET_ERROR);
        }
        */
        GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);
        break;
    default:
        break;
    }
    sleep(3);
    GPIO_toggle(CONFIG_LED_1_GPIO);
}


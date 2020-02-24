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
        sendMsgToUARTTxQ(GET_FIRMWARE, EMPTY);
        sendMsgToUARTTxQ(DEVICE_ID, GET_CONFIG);
        sendMsgToUARTTxQ(PWM_PARAM, GET_CONFIG);
        sendMsgToUARTTxQ(SHUTDOWN_ON_ERROR, GET_CONFIG);
        sendMsgToUARTTxQ(SERIAL_TIMEOUT, GET_CONFIG);
        *num = test1;
        break;
    case test1:
        //left motor forward half speed
        //sendLeftForwardMsgToMotorsQ(0x7F);
        //sendRightForwardMsgToMotorsQ(0x00);
        sendMsgToUARTTxQ(0x7F, M0_FORWARD);
        sendMsgToUARTTxQ(0x00, M1_FORWARD);
        *num = test2;
        break;
    case test2:
        //right motor forward half speed
        //sendLeftForwardMsgToMotorsQ(0x00);
        //sendRightForwardMsgToMotorsQ(0x7F);
        sendMsgToUARTTxQ(0x00, M0_FORWARD);
        sendMsgToUARTTxQ(0x7F, M1_FORWARD);
        *num = test3;
        break;
    case test3:
        //both motors forward half speed
        //sendLeftForwardMsgToMotorsQ(0x7F);
        //sendRightForwardMsgToMotorsQ(0x7F);
        sendMsgToUARTTxQ(0x7F, M0_FORWARD);
        sendMsgToUARTTxQ(0x7F, M1_FORWARD);
        *num = test4;
        break;
    case test4:
        //both motors forward max speed
        //sendLeftForwardMsgToMotorsQ(0x7F);
        //sendRightForwardMsgToMotorsQ(0x7F);
        sendMsgToUARTTxQ(0x7F, M0_FORWARD_8BIT);
        sendMsgToUARTTxQ(0x7F, M1_FORWARD_8BIT);
        *num = test5;
        break;
    case test5:
        //both motors reverse half speed
        //sendLeftReverseMsgToMotorsQ(0x7F);
        //sendRightReverseMsgToMotorsQ(0x7F);
        sendMsgToUARTTxQ(0x7F, M0_REVERSE);
        sendMsgToUARTTxQ(0x7F, M1_REVERSE);
        *num = test6;
        break;
    case test6:
        //both motors reverse max speed
        //sendLeftReverseMsgToMotorsQ(0x7F);
        //sendRightReverseMsgToMotorsQ(0x7F);
        sendMsgToUARTTxQ(0x7F, M0_REVERSE);
        sendMsgToUARTTxQ(0x7F, M1_REVERSE);
        *num = test7;
        break;
    case test7:
        //turn right
        //sendLeftForwardMsgToMotorsQ(0x7F);
        //sendRightReverseMsgToMotorsQ(0x7F);
        sendMsgToUARTTxQ(0x7F, M0_FORWARD_8BIT);
        sendMsgToUARTTxQ(0x7F, M1_REVERSE);
        *num = test8;
        break;
    case test8:
        //turn left
        //sendLeftReverseMsgToMotorsQ(0x7F);
        //sendRightForwardMsgToMotorsQ(0x7F);
        sendMsgToUARTTxQ(0x7F, M0_REVERSE);
        sendMsgToUARTTxQ(0x7F, M1_FORWARD_8BIT);
        *num = test9;
        break;
    case test9:
        //Decel & Accel
        //sendDecelMsgToMotorsQ(0x3F);
        //sleep(3);
        //sendAccelMsgToMotorsQ(0x3F);
        *num = test10;
        break;
    case test10:
        //Pause & Resume
        //sendPauseMsgToMotorsQ();
        //sleep(3);
        //sendResumeMsgToMotorsQ();
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


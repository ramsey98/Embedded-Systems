/*
 * test.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "test.h"

void *testThread(void *arg0)
{
    test_num num = test1;
    while(1)
    {
        tests(&num);
    }
}

void tests(test_num* num)
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
        *num = test3;
        break;
    case test3:
        //right motor forward max speed
        sendMsgToUARTTxQ(0x00, M0_FORWARD);
        sendMsgToUARTTxQ(0x7F, M1_FORWARD);
        *num = test4;
        break;
    case test4:
        //both motors forward max speed
        sendLeftForwardMsgToMotorsQ(0x7F);
        sendRightForwardMsgToMotorsQ(0x7F);
        *num = test5;
        break;
    case test5:
        //both motors reverse max speed
        sendLeftReverseMsgToMotorsQ(0x7F);
        sendRightReverseMsgToMotorsQ(0x7F);
        *num = test6;
        break;
    case test6:
        //both motors forward half speed
        sendLeftForwardMsgToMotorsQ(0x3F);
        sendRightForwardMsgToMotorsQ(0x3F);
        *num = test7;
        break;
    case test7:
        //turn right then left
        sendLeftForwardMsgToMotorsQ(0x7F);
        sendRightReverseMsgToMotorsQ(0x3F);
        sleep(3);
        sendLeftReverseMsgToMotorsQ(0x3F);
        sendRightForwardMsgToMotorsQ(0x7F);
        sleep(3);
        *num = test8;
        break;
    case test8:
        //Accel then Decel
        sendLeftForwardMsgToMotorsQ(0x0);
        sendRightForwardMsgToMotorsQ(0x0);
        sendAccelMsgToMotorsQ(25);
        sleep(1);
        sendAccelMsgToMotorsQ(25);
        sleep(1);
        sendAccelMsgToMotorsQ(25);
        sleep(1);
        sendAccelMsgToMotorsQ(25);
        sleep(1);
        sendAccelMsgToMotorsQ(25);
        sleep(1);
        sendDecelMsgToMotorsQ(25);
        sleep(1);
        sendDecelMsgToMotorsQ(25);
        sleep(1);
        sendDecelMsgToMotorsQ(25);
        sleep(1);
        sendDecelMsgToMotorsQ(25);
        sleep(1);
        sendDecelMsgToMotorsQ(25);
        *num = test9;
        break;
    case test9:
        //Pause & Resume
        sendLeftForwardMsgToMotorsQ(0x3F);
        sendRightForwardMsgToMotorsQ(0x3F);
        sendPauseMsgToMotorsQ();
        sleep(3);
        sendResumeMsgToMotorsQ();
        sleep(3);
        *num = test10;
        break;
    case test10:
        //test Capture GPIO
        sendLeftForwardMsgToMotorsQ(0x7F);
        sendRightForwardMsgToMotorsQ(0x7F);
        sleep(3);
    case end:
        /*
        if(error_gpio == high)
        {
            sendMsgToMQTTQ(GET_ERROR);
        }
        */
        GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);
        break;
    default:
        break;
    }
    GPIO_toggle(CONFIG_LED_1_GPIO);
    sendMsgToUARTTxQ(0x00, M0_FORWARD);
    sendMsgToUARTTxQ(0x00, M1_FORWARD);

}


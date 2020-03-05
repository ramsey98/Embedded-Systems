/*
 * tests.c
 *
 *  Created on: Feb 26, 2020
 *      Author: jared
 */

#include "tests.h"
#include "ti_drivers_config.h"

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
        // Turn LEDs off
        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
        GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
        GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_OFF);

//        //Interface with controller
//        sendMsgToUARTTxQ(GET_FIRMWARE, EMPTY);
//        sendMsgToUARTTxQ(DEVICE_ID, GET_CONFIG);
//        sendMsgToUARTTxQ(PWM_PARAM, GET_CONFIG);
//        sendMsgToUARTTxQ(SHUTDOWN_ON_ERROR, GET_CONFIG);
//        sendMsgToUARTTxQ(SERIAL_TIMEOUT, GET_CONFIG);
//
        *num = test2;
        break;

    case test2:
        // Turn left using all wheels for 2 seconds
        // At full speed (127)

        sendStartMsgToEncoderQueue();
        sendMsgToEncoderQueue();

        sendMsgToMCQ(0x7F04); //turn left
        sendMsgToEncoderQueue();
        sleep(2);
        sendMsgToEncoderQueue();
        sendMsgToMCQ(0x0); //stop

        sendMsgToEncoderQueue();

        *num = test3;
        break;

    case test3:
        // Repeat test2 this time turning right
        // At full speed (127)

        sendMsgToEncoderQueue();
        sendMsgToMCQ(0xFF04); //turn right
        sendMsgToEncoderQueue();
        sleep(2);
        sendMsgToEncoderQueue();
        sendMsgToMCQ(0x0); //stop
        sendMsgToEncoderQueue();

        *num = test4;
        break;

    case test4:
        // Move forwards for two seconds towards motor 1
        // Half speed (64)

        sendMsgToMCQ(0x3F08); //move 1
        sleep(2);
        sendMsgToMCQ(0x0); //stop

        *num = test5;
        break;

    case test5:
        // Move forwards for two seconds towards motor 2
        // Half speed (64)

        sendMsgToMCQ(0x3F09); //move 2
        sleep(2);
        sendMsgToMCQ(0x0); //stop

        *num = test6;
        break;

    case test6:
           // Move forwards for two seconds towards motor 3
           // Half speed (64)

        sendMsgToMCQ(0x3F0A); //move 3
        sleep(2);
        sendMsgToMCQ(0x0); //stop

        *num = test7;
        break;

    case test7:
        // Move backwards for two seconds away from motor 1
        // Half speed (64)

        sendMsgToMCQ(0xBF08); //move 1
        sleep(2);
        sendMsgToMCQ(0x0); //stop

        *num = test8;
        break;

    case test8:
        // Move backwards for two seconds away from motor 2
        // Half speed (64)

        sendMsgToMCQ(0xBF09); //move 2
        sleep(2);
        sendMsgToMCQ(0x0); //stop

        *num = test9;
        break;

    case test9:
        // Move backwards for two seconds away from motor 3
       // Half speed (64)

        sendMsgToMCQ(0xBF0A); //move 3
        sleep(2);
        sendMsgToMCQ(0x0); //stop

        *num = test10;
        break;

    case test10:
        // Move motor 1 wheel individually for one second in each direction
        // Sleep for one second in between
        // Half speed (64) first, then full speed (127)

        sendMsgToMCQ(0xBF0C); //move 1
        sleep(1);
        sendMsgToMCQ(0x0); //stop
        sleep(1);

        sendMsgToMCQ(0x7F0C); //move 1
        sleep(1);
        sendMsgToMCQ(0x0); //stop

        *num = test11;
        break;

    case test11:
        // Move motor 2 wheel individually for one second in each direction
        // Sleep for one second in between
        // Half speed (64) first, then full speed (127)

        sendMsgToMCQ(0xBF0D); //move 2
        sleep(1);
        sendMsgToMCQ(0x0); //stop
        sleep(1);

        sendMsgToMCQ(0x7F0D); //move 2
        sleep(1);
        sendMsgToMCQ(0x0); //stop

        *num = test12;
        break;

    case test12:
        // Move motor 3 wheel individually for one second in each direction
        // Sleep for one second in between
        // Half speed (64) first, then full speed (127)

        sendMsgToMCQ(0xBF0E); //move 3
        sleep(1);
        sendMsgToMCQ(0x0); //stop
        sleep(1);

        sendMsgToMCQ(0x7F0E); //move 3
        sleep(1);
        sendMsgToMCQ(0x0); //stop

        *num = end;
        break;

    case end:
        GPIO_toggle(CONFIG_LED_1_GPIO);
        break;
    default:
        break;
    }
    sleep(2);
    //GPIO_toggle(CONFIG_LED_1_GPIO);
}






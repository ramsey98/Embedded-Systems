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
    test_num num = test2;
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
        //both motors forward max speed
//        moveRover(0x4F);
        sleep(3);
        GPIO_toggle(CONFIG_LED_0_GPIO);
        *num = test1;
        break;
    case test2:
        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
        *num = end;
        break;
    case end:
        GPIO_toggle(CONFIG_LED_0_GPIO);
        GPIO_toggle(CONFIG_LED_1_GPIO);
        break;
    default:
        break;
    }
    sleep(1);
    //GPIO_toggle(CONFIG_LED_1_GPIO);
}






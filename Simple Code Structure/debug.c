/*
 * debug.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */
#include "debug.h"

void dbgUARTVal(unsigned char outVal)
{

}

void dbgOutputLoc(unsigned int outLoc)
{

}

void halt()
{
    dbgOutputLoc(DBG_ERROR);
    GPIO_init();
    GPIO_setConfig(CONFIG_LED_0_GPIO, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
}

/*
 * Main.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */
#include "debug.h"
#include "sensor_state.h"

void *mainThread(void *arg0)
{
    UART_init();

    GPIO_init();
    GPIO_setConfig(CONFIG_GPIO_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    curState = Init;

    while(1)
    {
        dbgOutputLoc(WHILE1);
    }

}






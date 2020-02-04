/*
 * debug.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */
#include "debug.h"

void dbgUARTInit()
{
    UART_init();
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;
}

void dbgGPIOInit()
{
    GPIO_init();
    GPIO_setConfig(CONFIG_GPIO_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_5, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_6, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_7, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}

void dbgUARTVal(unsigned char outVal)
{
    uart = UART_open(CONFIG_UART_0, &uartParams);
    UART_write(uart, &outVal, sizeof(outVal));
}

void dbgUARTStr(char uartOut[])
{
    uart = UART_open(CONFIG_UART_0, &uartParams);
    UART_write(uart, uartOut, sizeof(uartOut));
}

void dbgOutputLoc(unsigned int outLoc)
{
    GPIO_write(CONFIG_GPIO_6, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_5, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_4, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_3, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_2, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_1, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_0, CONFIG_GPIO_LED_OFF);
    if (outLoc <= 127)
    {
        if (outLoc & 0b100000)
        {
            GPIO_write(CONFIG_GPIO_6, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b100000)
        {
            GPIO_write(CONFIG_GPIO_5, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b010000)
        {
            GPIO_write(CONFIG_GPIO_4, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b001000)
        {
            GPIO_write(CONFIG_GPIO_3, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b000100)
        {
            GPIO_write(CONFIG_GPIO_2, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b000010)
        {
            GPIO_write(CONFIG_GPIO_1, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b000001)
        {
            GPIO_write(CONFIG_GPIO_0, CONFIG_GPIO_LED_ON);
        }
    }
    GPIO_toggle(CONFIG_GPIO_7);
}

void halt()
{
    GPIO_write(CONFIG_GPIO_0, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_1, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_2, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_3, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_4, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_5, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_6, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_7, CONFIG_GPIO_LED_ON);
    vTaskSuspendAll();
    while(1) {}
}

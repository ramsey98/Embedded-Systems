/*
 * debug.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */
#include "debug.h"

void dbgUARTVal(unsigned char outVal)
{

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(CONFIG_UART_0, &uartParams);
    UART_write(uart, &outVal, sizeof(outVal));
}

void dbgUARTStr(char uartOut[])
{

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

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
        if (outLoc & 0b100000 > 0)
        {
            GPIO_write(CONFIG_GPIO_6, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b100000 > 0)
        {
            GPIO_write(CONFIG_GPIO_5, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b010000 > 0)
        {
            GPIO_write(CONFIG_GPIO_4, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b001000 > 0)
        {
            GPIO_write(CONFIG_GPIO_3, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b000100 > 0)
        {
            GPIO_write(CONFIG_GPIO_2, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b000010 > 0)
        {
            GPIO_write(CONFIG_GPIO_1, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b000001 > 0)
        {
            GPIO_write(CONFIG_GPIO_0, CONFIG_GPIO_LED_ON);
        }
        GPIO_toggle(CONFIG_GPIO_7);
    }
    else
    {
        halt();
    }
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
}

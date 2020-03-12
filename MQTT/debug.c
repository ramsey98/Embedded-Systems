/*
 * debug.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */
#include "debug.h"
#include <string.h>
#include <stdio.h>
#include <ti/drivers/dpl/HwiP.h>
static UART_Handle uart;

void dbgUARTInit()
{
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.baudRate = 38400;
    uartParams.readEcho = UART_ECHO_OFF;
    uart = UART_open(CONFIG_UART_1, &uartParams);
    if (uart == NULL)
    {
        ERROR;
    }
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
    GPIO_setConfig(CONFIG_LED_0_GPIO, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_LED_1_GPIO, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}

void dbgUARTVal(unsigned char outVal)
{
    UART_write(uart, &outVal, sizeof(outVal));
}

void dbgUARTStr(const char * uartOut)
{
    int i;
    for(i = 0; i < strlen(uartOut); i++)
    {
        UART_write(uart, &uartOut[i], sizeof(uartOut[i]));
    }
}

void dbgUARTNum(int outVal)
{
    char str[3];
    if (outVal > 255)
    {
        sprintf(str, "%d", outVal);
        dbgUARTStr(str);
    }
    else
    {
        dbgUARTVal(outVal);
    }
}

void dbgOutputLoc(unsigned int outLoc)
{
    GPIO_write(CONFIG_GPIO_7, CONFIG_GPIO_LED_OFF);
    GPIO_toggle(CONFIG_GPIO_7); //when GPIO_7 is high, bits are being switched
    GPIO_write(CONFIG_GPIO_6, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_5, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_4, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_3, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_2, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_1, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_0, CONFIG_GPIO_LED_OFF);

    if (outLoc <= 127)
    {
        if (outLoc & 0b01000000)
        {
            GPIO_write(CONFIG_GPIO_6, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b00100000)
        {
            GPIO_write(CONFIG_GPIO_5, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b00010000)
        {
            GPIO_write(CONFIG_GPIO_4, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b00001000)
        {
            GPIO_write(CONFIG_GPIO_3, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b00000100)
        {
            GPIO_write(CONFIG_GPIO_2, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b00000010)
        {
            GPIO_write(CONFIG_GPIO_1, CONFIG_GPIO_LED_ON);
        }
        if (outLoc & 0b00000001)
        {
            GPIO_write(CONFIG_GPIO_0, CONFIG_GPIO_LED_ON);
        }

        GPIO_toggle(CONFIG_GPIO_7);
    }
    else
    {
        ERROR;
    }
}

void halt(int line, const char* func)
{
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
    dbgUARTStr("Error in func: ");
    dbgUARTStr(func);
    dbgUARTStr(" at line #: ");
    dbgUARTNum(line);
    HwiP_disable();
    vTaskSuspendAll();
    int timerCount = 0;
    while(1)
    {
        timerCount++;
        if (timerCount > DBG_ERROR_LED_TIME)
        {
            timerCount = 0;
            GPIO_toggle(CONFIG_LED_0_GPIO);
        }
    }
}

/*
 * motors.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "motors.h"

static UART_Handle motors_uart;

void motorsUARTInit()
{
    UART_init();
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.baudRate = 115200;
    uartParams.readEcho = UART_ECHO_OFF;
    motors_uart = UART_open(CONFIG_UART_1, &uartParams);
    if (motors_uart == NULL)
    {
        halt();
    }
    writeMotors(INIT_CONTROLLER);
}

void writeMotors(unsigned char outVal)
{
    UART_write(motors_uart, &outVal, sizeof(outVal));
}

char readMotors()
{
    char input;
    UART_read(motors_uart, &input, sizeof(char));
    return input;
}


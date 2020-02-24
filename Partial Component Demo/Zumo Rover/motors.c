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
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 38400;
    uartParams.readEcho = UART_ECHO_OFF;
    motors_uart = UART_open(CONFIG_UART_0, &uartParams);
    if (motors_uart == NULL)
    {
        ERROR;
    }
    sendMsgToUARTTxQ(INIT_CONTROLLER, EMPTY);
}

void *motorsThread(void *arg0)
{
    motorsUARTInit();
    dbgOutputLoc(ENTER_TASK);
    MOTORS_DATA motorsState;
    motorsState.state = Motors_Init;
    uint8_t type = 0;
    uint8_t value = 0;
    int received = 0;
    int success = motors_fsm(&motorsState, type, value);
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromMotorsQ(&type, &value);
        success = motors_fsm(&motorsState, type, value);
        if(received == -1 || success == -1)
        {
            ERROR;
        }
        else if(motorsState.state == Motors_Ready)
        {
            if(motorsState.leftDir == 0)
            {
                sendMsgToUARTTxQ(motorsState.leftSpeed, M0_FORWARD_8BIT);
            }
            else
            {
                sendMsgToUARTTxQ(motorsState.leftSpeed, M0_REVERSE_8BIT);
            }
            if(motorsState.rightDir == 0)
            {
                sendMsgToUARTTxQ(motorsState.rightSpeed, M1_FORWARD_8BIT);
            }
            else
            {
                sendMsgToUARTTxQ(motorsState.rightSpeed, M1_REVERSE_8BIT);
            }
        }
    }
}

void *UARTTxThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint8_t byte1;
    uint16_t value;
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromUARTTxQ(&value);
        if(received == -1)
        {
            ERROR;
        }
        else
        {
            byte1 = value & 0xFF;
            if(byte1 == 0)
            {
                byte1 = value >> 8;
                UART_write(motors_uart, &byte1, sizeof(byte1));
            }
            else
            {
                UART_write(motors_uart, &value, sizeof(value));
            }
        }
    }
}

void *UARTRxThread(void *arg0)
{
    int_fast32_t ret;
    char value;
    while(1)
    {
        ret = UART_read(motors_uart, &value, sizeof(value));
        if(ret == UART_STATUS_ERROR)
        {
            ERROR;
        }
        else
        {
            ret = sendMsgToUARTRxQ(value);
        }
    }
}

void *UARTOutThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint8_t value;
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromUARTRxQ(&value);
        if(received == -1)
        {
            ERROR;
        }
        else
        {
            dbgUARTStr("Controller Value Received:");
            dbgUARTVal(value);
        }
    }
}

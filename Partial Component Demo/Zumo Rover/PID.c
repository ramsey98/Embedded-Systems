/*
 * motors.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include <PID.h>

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
    int sent = sendMsgToUARTTxQ(INIT_CONTROLLER, EMPTY);
    if(sent == -1)
    {
        ERROR;
    }
}

void updateMotors(MOTORS_DATA motorsState)
{
    int sent = 0;
    if(motorsState.state == Motors_Ready)
    {
        if(motorsState.leftDir == 0)
        {
            sent = sendMsgToUARTTxQ(motorsState.leftSpeed, M0_FORWARD_8BIT);
        }
        else
        {
            sent = sendMsgToUARTTxQ(motorsState.leftSpeed, M0_REVERSE_8BIT);
        }
        if(motorsState.rightDir == 0)
        {
            sent = sendMsgToUARTTxQ(motorsState.rightSpeed, M1_FORWARD_8BIT);
        }
        else
        {
            sent = sendMsgToUARTTxQ(motorsState.rightSpeed, M1_REVERSE_8BIT);
        }
    }
    else if(motorsState.state == Motors_Paused)
    {
        sent = sendMsgToUARTTxQ(0, M0_FORWARD_8BIT);
        sent = sendMsgToUARTTxQ(0, M1_FORWARD_8BIT);
    }
    if(sent == -1)
    {
        ERROR;
    }
}

void *PIDThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    motorsUARTInit();
    int received = 0, success = 0, leftCount = 0, rightCount = 0;
    uint32_t type = 0, value = 0;
    MOTORS_DATA motorsState;
    success = motors_fsm(&motorsState, type, value);
    if(success == -1)
    {
        ERROR;
    }
    while(1)
    {
        received = receiveFromPIDQ(&type, &value);
        if(received == -1)
        {
            ERROR;
        }
        else
        {
            success = motors_fsm(&motorsState, type, value);
            if(success == -1)
            {
                ERROR;
            }
            else if(type == TIMER)
            {
                updateMotors(motorsState);
            }
            else if(type == LEFTCAP)
            {
                sendLeftCapMsgToUARTDebugQ(value);
            }
            else if(type == RIGHTCAP)
            {
                sendRightCapMsgToUARTDebugQ(value);
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

void *UARTDebugThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint16_t value, type;
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromUARTDebugQ(&type, &value);
        if(received == -1)
        {
            ERROR;
        }
        else
        {
            if(type == LEFTCAP)
            {
                dbgUARTStr("Left Motor:");
            }
            else if(type == RIGHTCAP)
            {
                dbgUARTStr("Right Motor:");
            }
            dbgUARTNum(value);
        }
    }
}

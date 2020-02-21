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
    uartParams.baudRate = 115200;
    uartParams.readEcho = UART_ECHO_OFF;
    motors_uart = UART_open(CONFIG_UART_0, &uartParams);
    if (motors_uart == NULL)
    {
        ERROR;
    }
    sendMsgToUARTTxQ(INIT_CONTROLLER);
}

void *motorsThread(void *arg0)
{
    createMotorsQueue();
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
        else
        {
            if(motorsState.leftDir == 0)
            {
                sendMsgToUARTTxQ(M0_FORWARD);
                sendMsgToUARTTxQ(motorsState.leftSpeed);
            }
            else
            {
                sendMsgToUARTTxQ(M0_REVERSE);
                sendMsgToUARTTxQ(motorsState.leftSpeed);
            }
            if(motorsState.rightDir == 0)
            {
                sendMsgToUARTTxQ(M1_FORWARD);
                sendMsgToUARTTxQ(motorsState.rightSpeed);
            }
            else
            {
                sendMsgToUARTTxQ(M1_REVERSE);
                sendMsgToUARTTxQ(motorsState.rightSpeed);
            }
        }
    }
}

void *UARTTxThread(void *arg0)
{
    createUARTTxQueue();
    dbgOutputLoc(ENTER_TASK);
    uint8_t value;
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
            UART_write(motors_uart, &value, sizeof(value));
        }
    }
}

void *UARTRxThread(void *arg0)
{
    createUARTRxQueue();
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
            dbgUARTStr("\n");
        }
    }
}

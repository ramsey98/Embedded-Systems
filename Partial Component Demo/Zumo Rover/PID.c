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
    if(motorsState.paused == 0)
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
    else if(motorsState.paused == 1)
    {
        sent = sendMsgToUARTTxQ(0, M0_FORWARD_8BIT);
        sent = sendMsgToUARTTxQ(0, M1_FORWARD_8BIT);
    }
    if(sent == -1)
    {
        ERROR;
    }
}

void updateValues(MOTORS_DATA *motorsState, uint32_t type, uint32_t value)
{
    switch(type)
    {
        case PAUSE:
        {
            motorsState->paused = 1;
        }
        case RESUME:
        {
            motorsState->paused = 0;
        }
        case TURNLEFT:
        {
            motorsState->leftDir = 0;
            motorsState->leftSpeed = value;
            motorsState->rightDir = 1;
            motorsState->rightSpeed = value/2;
        }
        case TURNRIGHT:
        {
            motorsState->leftDir = 1;
            motorsState->leftSpeed = value/2;
            motorsState->rightDir = 0;
            motorsState->rightSpeed = value;
        }
        case FORWARD:
        {
            motorsState->leftDir = 0;
            motorsState->leftSpeed = value;
            motorsState->rightDir = 0;
            motorsState->rightSpeed = value;
        }
        case REVERSE:
        {
            motorsState->leftDir = 1;
            motorsState->leftSpeed = value;
            motorsState->rightDir = 1;
            motorsState->rightSpeed = value;
        }
        case ACCEL:
        {
            if((motorsState->leftSpeed + value) < 255)
            {
                motorsState->leftSpeed += value;
            }
            else
            {
                motorsState->leftSpeed = 255;
            }
            if((motorsState->rightSpeed + value) < 255)
            {
                motorsState->rightSpeed += value;
            }
            else
            {
                motorsState->rightSpeed = 255;
            }
        }
        case DECEL:
        {
            if(motorsState->leftSpeed > value)
            {
                motorsState->leftSpeed -= value;
            }
            else
            {
                motorsState->leftSpeed = 0;
            }
            if(motorsState->rightSpeed > value)
            {
                motorsState->rightSpeed -= value;
            }
            else
            {
                motorsState->rightSpeed = 0;
            }
        }
        default:
        {
            ERROR;
        }
    }
}

void *PIDThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    motorsUARTInit();
    int received = 0, leftCount = 0, rightCount = 0;
    uint32_t type = 0, value = 0;
    MOTORS_DATA motorsState;
    motorsState.leftSpeed = 0;
    motorsState.rightSpeed = 0;
    motorsState.leftDir = 0;
    motorsState.rightDir = 0;
    motorsState.paused = 0;
    while(1)
    {
        received = receiveFromPIDQ(&type, &value);
        if(received == -1)
        {
            ERROR;
        }
        else
        {
            if(type == TIMER)
            {
                if(leftCount != getLeftCount() | rightCount != getRightCount())
                {
                    ERROR;
                }
                updateMotors(motorsState);
                leftCount = 0;
                rightCount = 0;
                sendMsgToUARTDebugQ(TIMER, value);
            }
            else if(type == LEFTCAP)
            {
                sendMsgToUARTDebugQ(LEFTCAP, value);
                leftCount++;
            }
            else if(type == RIGHTCAP)
            {
                sendMsgToUARTDebugQ(RIGHTCAP, value);
                rightCount++;
            }
            else
            {
                updateValues(&motorsState, type, value);
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

/*
 * PID.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "PID.h"

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

void updateMotors(MOTORS_DATA motorsState)
{
    if(motorsState.paused == 0)
    {
        if(motorsState.leftDir == 0)
        {
            sendMsgToUARTTxQ(motorsState.setLeftSpeed, M0_FORWARD_8BIT);
        }
        else
        {
            sendMsgToUARTTxQ(motorsState.setLeftSpeed, M0_REVERSE_8BIT);
        }
        if(motorsState.rightDir == 0)
        {
            sendMsgToUARTTxQ(motorsState.setRightSpeed, M1_FORWARD_8BIT);
        }
        else
        {
            sendMsgToUARTTxQ(motorsState.setRightSpeed, M1_REVERSE_8BIT);
        }
    }
    else if(motorsState.paused == 1)
    {
        sendMsgToUARTTxQ(0, M0_FORWARD_8BIT);
        sendMsgToUARTTxQ(0, M1_FORWARD_8BIT);
    }
}

void updateValues(MOTORS_DATA *motorsState, uint32_t type, uint32_t value)
{
    //this is the PID adjustment
    /*
    if(motorsState->setLeftSpeed < motorsState->realLeftSpeed)
    {
        motorsState->setLeftSpeed--;
    }
    else if(motorsState->setLeftSpeed > motorsState->realLeftSpeed)
    {
        motorsState->setLeftSpeed++;
    }
    if(motorsState->setLeftSpeed < motorsState->realRightSpeed)
    {
        motorsState->setRightSpeed--;
    }
    else if(motorsState->setRightSpeed > motorsState->realRightSpeed)
    {
        motorsState->setRightSpeed++;
    }
    */
    switch(type)
    {
        case PAUSE:
        {
            motorsState->paused = 1;
            break;
        }
        case RESUME:
        {
            motorsState->paused = 0;
            break;
        }
        case TURNLEFT:
        {
            motorsState->leftDir = 0;
            motorsState->setLeftSpeed = value;
            motorsState->rightDir = 1;
            motorsState->setRightSpeed = value/2;
            break;
        }
        case TURNRIGHT:
        {
            motorsState->leftDir = 1;
            motorsState->setLeftSpeed = value/2;
            motorsState->rightDir = 0;
            motorsState->setRightSpeed = value;
            break;
        }
        case FORWARD:
        {
            motorsState->leftDir = 0;
            motorsState->setLeftSpeed = value;
            motorsState->rightDir = 0;
            motorsState->setRightSpeed = value;
            break;
        }
        case REVERSE:
        {
            motorsState->leftDir = 1;
            motorsState->setLeftSpeed = value;
            motorsState->rightDir = 1;
            motorsState->setRightSpeed = value;
            break;
        }
        case ACCEL:
        {
            if((motorsState->setLeftSpeed + value) < 255)
            {
                motorsState->setLeftSpeed += value;
            }
            else
            {
                motorsState->setLeftSpeed = 255;
            }
            if((motorsState->setRightSpeed + value) < 255)
            {
                motorsState->setRightSpeed += value;
            }
            else
            {
                motorsState->setRightSpeed = 255;
            }
            break;
        }
        case DECEL:
        {
            if(motorsState->setLeftSpeed > value)
            {
                motorsState->setLeftSpeed -= value;
            }
            else
            {
                motorsState->setLeftSpeed = 0;
            }
            if(motorsState->setRightSpeed > value)
            {
                motorsState->setRightSpeed -= value;
            }
            else
            {
                motorsState->setRightSpeed = 0;
            }
            break;
        }
        default:
        {
            ERROR;
            break;
        }
    }
}

void *PIDThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    motorsUARTInit();
    int leftCount = 0, rightCount = 0;
    uint32_t type = 0, value = 0;
    MOTORS_DATA motorsState;
    motorsState.setLeftSpeed = 0;
    motorsState.setRightSpeed = 0;
    motorsState.realLeftSpeed = 0;
    motorsState.realRightSpeed = 0;
    motorsState.leftDir = 0;
    motorsState.rightDir = 0;
    motorsState.paused = 0;
    while(1)
    {
        receiveFromPIDQ(&type, &value);
        switch(type)
        {
            case TIMER:
            {
                if(leftCount != getLeftCount() | rightCount != getRightCount())
                {
                    ERROR;
                }
                clearCounts();
                updateMotors(motorsState);
                sendMsgToUARTDebugQ(TIMER, value);
                sendMsgToUARTDebugQ(LEFTCAP, motorsState.realLeftSpeed);
                sendMsgToUARTDebugQ(RIGHTCAP, motorsState.realRightSpeed);
                sendMsgToUARTDebugQ(LEFTCOUNT, leftCount);
                sendMsgToUARTDebugQ(RIGHTCOUNT, rightCount);
                leftCount = 0;
                rightCount = 0;
                break;
            }
            case LEFTCAP:
            {
                //1.6ms is full speed, need to measure min speed
                motorsState.realLeftSpeed = value;
                leftCount++;
                break;
            }
            case RIGHTCAP:
            {
                motorsState.realRightSpeed = value;
                rightCount++;
                break;
            }
            default:
            {
                updateValues(&motorsState, type, value);
                break;
            }
        }
    }
}

void *UARTTxThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint8_t byte1;
    uint16_t value;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromUARTTxQ(&value);
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

/*
 * PID.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "PID.h"

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
            motorsState->leftDir = 1;
            motorsState->setLeftSpeed = value/2;
            motorsState->rightDir = 0;
            motorsState->setRightSpeed = value;
            break;
        }
        case TURNRIGHT:
        {
            motorsState->leftDir = 0;
            motorsState->setLeftSpeed = value;
            motorsState->rightDir = 1;
            motorsState->setRightSpeed = value/2;
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

void PIDEvent(MOTORS_DATA *motorsState, uint32_t type, uint32_t value)
{
    static int leftCount = 0, rightCount = 0;
    switch(type)
    {
        case TIMER:
        {
            if(leftCount != getLeftCount()) ERROR;
            if(rightCount != getRightCount()) ERROR;
            clearCounts();
            updateMotors(*motorsState);
            sendMsgToUARTDebugQ(LEFTCAP, motorsState->realLeftSpeed);
            sendMsgToUARTDebugQ(RIGHTCAP, motorsState->realRightSpeed);
            sendMsgToUARTDebugQ(LEFTCOUNT, leftCount);
            sendMsgToUARTDebugQ(RIGHTCOUNT, rightCount);
            leftCount = 0;
            rightCount = 0;
            break;
        }
        case LEFTCAP:
        {
            //1.6ms is full speed, need to measure min speed
            motorsState->realLeftSpeed = value;
            leftCount++;
            break;
        }
        case RIGHTCAP:
        {
            motorsState->realRightSpeed = value;
            rightCount++;
            break;
        }
        case SENSOR:
        {
            if(value > 65)
            {
                updateValues(motorsState, ACCEL, value - 65);
            }
            else if(value < 55)
            {
                updateValues(motorsState, DECEL, 55 - value);
            }
        }
        case PIXY:
        {
            //updateValue(motorsState, TURNLEFT, 60);
            //updateValue(motorsState, TURNRIGHT, 60);
        }
        default:
        {
            updateValues(motorsState, type, value);
            break;
        }
    }
}

void *PIDThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint32_t type = 0, value = 0;
    MOTORS_DATA motorsState = {.setLeftSpeed = 0,
                               .setRightSpeed = 0,
                               .realLeftSpeed = 0,
                               .realRightSpeed = 0,
                               .leftDir = 0,
                               .rightDir = 0,
                               .paused = 0};
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromPIDQ(&type, &value);
        PIDEvent(&motorsState, type, value);
    }
}

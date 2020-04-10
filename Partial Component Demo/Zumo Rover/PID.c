/*
 * PID.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "PID.h"

const PIDLookupTable PIDLookup[PIDLOOKUPLEN] = {{0,4}, //{microvolt, in}
                                                {10,6},
                                                {20,8},
                                                {30,10},
                                                {40,12},
                                                {50,14},
                                                {60,16},
                                                {70,18},
                                                {80,20},
                                                {90,22},
                                                {100,24},
                                                {120,26},
                                                {127,28}};

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

void PIDAdjust(MOTORS_DATA *motorsState)
{
    int i;
    for(i = 0; i < PIDLOOKUPLEN; i++)
    {
        if(motorsState->realLeftSpeed <= PIDLookup[i].measured)
        {
            motorsState->setLeftSpeed = PIDLookup[i].expected;
        }
        if(motorsState->realRightSpeed <= PIDLookup[i].measured)
        {
            motorsState->setRightSpeed = PIDLookup[i].expected;
        }
    }
    //updateMotors(*motorsState); //or just update speeds?
}

void updateValues(MOTORS_DATA *motorsState, uint32_t type, uint32_t value)
{
    //PIDAdjust(motorsState);
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
            if((motorsState->setLeftSpeed + value) < 127)
            {
                motorsState->setLeftSpeed += value;
            }
            else
            {
                motorsState->setLeftSpeed = 127;
            }
            if((motorsState->setRightSpeed + value) < 127)
            {
                motorsState->setRightSpeed += value;
            }
            else
            {
                motorsState->setRightSpeed = 127;
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
    switch(type)
    {
        case TIMER:
        {
            updateMotors(*motorsState);
            motorsState->realLeftSpeed = 0;
            motorsState->realRightSpeed = 0;
            break;
        }
        case LEFTCAP:
        {
            motorsState->realLeftSpeed = value;
            break;
        }
        case RIGHTCAP:
        {
            motorsState->realRightSpeed = value;
            break;
        }
        case SENSOR:
        {
            if(value >= 20)
            {
                updateValues(motorsState, ACCEL, value - 12);
            }
            else if(value <= 12 & value >= 8)
            {
                updateValues(motorsState, DECEL, 12 - value);
            }
            else if(value < 8)
            {
                updateValues(motorsState, FORWARD, 0);
            }
            break;
        }
        case PIXY:
        {
            if(value == SYNCING)
            {
                sendMsgToUARTDebugQ(PIXY, SYNCING);
                //updateValues(motorsState, PAUSE, EMPTY);
            }
            else if(value != SYNCING)
            {
                //updateValue(motorsState, TURNLEFT, 60);
                //updateValue(motorsState, TURNRIGHT, 60);
                sendMsgToUARTDebugQ(PIXY, EMPTY);
            }
            break;
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

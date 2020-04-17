/*
 * PID.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include <navigation.h>
static int enablePID = 0, enableSensor = 0, enablePixy = 1;

const naviLookupTable naviLookup[NAVILOOKUPLEN] = {{0,0}, //{expected, measured}
                                                {10,0},
                                                {20,18000},
                                                {30,10000},
                                                {40,7000},
                                                {50,5400},
                                                {60,4400},
                                                {70,3700},
                                                {80,3200},
                                                {90,2800},
                                                {100,2500},
                                                {110,2290},
                                                {120,2100},
                                                {127,1940}};

void updateMotors(MOTORS_DATA motorsState)
{
    if(motorsState.paused == 0)
    {
        if(motorsState.leftDir == 0)
        {
            sendMsgToUARTTxQ(motorsState.realLeftSpeed, M0_FORWARD_8BIT);
        }
        else
        {
            sendMsgToUARTTxQ(motorsState.realLeftSpeed, M0_REVERSE_8BIT);
        }
        if(motorsState.rightDir == 0)
        {
            sendMsgToUARTTxQ(motorsState.realRightSpeed, M1_FORWARD_8BIT);
        }
        else
        {
            sendMsgToUARTTxQ(motorsState.realRightSpeed, M1_REVERSE_8BIT);
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
            motorsState->rightDir = 0;
            if(motorsState->setLeftSpeed > value)
            {
                motorsState->setLeftSpeed -= value;
            }
            else
            {
                motorsState->setLeftSpeed = 0;
            }
            if(motorsState->setRightSpeed + value > 127)
            {
                motorsState->setRightSpeed = 127;
            }
            else
            {
                motorsState->setRightSpeed += value;
            }
            break;
        }
        case TURNRIGHT:
        {
            motorsState->leftDir = 0;
            motorsState->rightDir = 0;
            if(motorsState->setRightSpeed > value)
            {
                motorsState->setRightSpeed -= value;
            }
            else
            {
                motorsState->setRightSpeed = 0;
            }
            if(motorsState->setLeftSpeed + value > 127)
            {
                motorsState->setLeftSpeed = 127;
            }
            else
            {
                motorsState->setLeftSpeed += value;
            }
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

//https://softwareengineering.stackexchange.com/questions/186124/programming-pid-loops-in-c
uint8_t PIDAdjust(uint8_t setSpeed, uint32_t measuredSpeed)
{
    uint8_t ret;
    if(enablePID == 1)
    {
        MQTTMsg msg = {.topic = JSON_TOPIC_DEBUG, .type = JSON_PID_BEFORE, .value = setSpeed};
        sendMsgToMQTTQ(msg);
        sendMsgToUARTDebugQ(PID_BEFORE, setSpeed);
        int i;
        static int error = 0, integral = 0;
        int PIDResult = 0, ret = 0;
        for(i = 0; i < NAVILOOKUPLEN; i++)
        {
            if(measuredSpeed >= naviLookup[i].measured)
            {
                error = naviLookup[i].expected - setSpeed;
            }
        }
        integral += error;
        PIDResult = (KP*error) + (KI*integral*measuredSpeed);
        if(setSpeed + PIDResult > 127)
        {
            ret = 127;
        }
        else if(setSpeed + PIDResult < 0)
        {
            ret = 0;
        }
        else
        {
            ret = setSpeed + PIDResult;
        }
        MQTTMsg msg2 = {.topic = JSON_TOPIC_DEBUG, .type = JSON_PID_AFTER, .value = setSpeed};
        sendMsgToMQTTQ(msg2);
        sendMsgToUARTDebugQ(PID_AFTER, ret);
    }
    else
    {
        ret = setSpeed;
    }
    return ret;
}

void naviEvent(MOTORS_DATA *motorsState, uint32_t type, uint32_t value)
{
    float scaled, halfway, diff;
    static MQTTMsg leftMsg = {.topic = JSON_TOPIC_DEBUG, .type = JSON_CAPTURE_LEFT};
    static MQTTMsg rightMsg = {.topic = JSON_TOPIC_DEBUG, .type = JSON_CAPTURE_RIGHT};
    switch(type)
    {
        case TIMER:
            updateMotors(*motorsState);
            motorsState->realLeftSpeed = motorsState->setLeftSpeed;
            motorsState->realRightSpeed = motorsState->setRightSpeed;
            sendMsgToMQTTQ(leftMsg);
            sendMsgToMQTTQ(rightMsg);
            leftMsg.value = 0;
            rightMsg.value = 0;
            break;
        case LEFTCAP:
            motorsState->measuredLeftSpeed = value;
            motorsState->realLeftSpeed = PIDAdjust(motorsState->setLeftSpeed, motorsState->measuredLeftSpeed);
            leftMsg.value = motorsState->measuredLeftSpeed;
            break;
        case RIGHTCAP:
            motorsState->measuredRightSpeed = value;
            motorsState->realRightSpeed = PIDAdjust(motorsState->setRightSpeed, motorsState->measuredRightSpeed);
            rightMsg.value = motorsState->measuredRightSpeed;
            break;
        case SENSOR:
            if(enableSensor == 1)
            {
                if(value >= 20)
                {
                    updateValues(motorsState, ACCEL, value - 12);
                }
                else if(value <= 12 & value >= 6)
                {
                    updateValues(motorsState, DECEL, ((12 - value)*2));
                }
                else if(value < 6)
                {
                    updateValues(motorsState, FORWARD, 0);
                }
            }
            break;
        case PIXY:
            if(enablePixy == 1)
            {
                halfway = (PIXY_X_RANGE/2.0);
                if(value < (PIXY_X_RANGE*.25))
                {
                    diff = halfway - value;
                    scaled = 127.0-(127.0*(diff / halfway));
                    updateValues(motorsState, TURNLEFT, scaled);
                }
                else if(value > (PIXY_X_RANGE*.75))
                {
                    diff = value - halfway;
                    scaled = 127.0*(diff / halfway);
                    updateValues(motorsState, TURNRIGHT, scaled);
                }
                else
                {
                    if(motorsState->setLeftSpeed > motorsState->setRightSpeed)
                    {
                        updateValues(motorsState, FORWARD, motorsState->setLeftSpeed);
                    }
                    else
                    {
                        updateValues(motorsState, FORWARD, motorsState->setRightSpeed);
                    }
                }
            }
            break;
        case PID_ENABLE:
            if(value == 0)
            {
                enablePID = 0;
            }
            else
            {
                enablePID = 1;
            }
            break;
        case SENSOR_ENABLE:
            if(value == 0)
            {
                enableSensor = 0;
            }
            else
            {
                enableSensor = 1;
            }
            break;
        case PIXY_ENABLE:
            if(value == 0)
            {
                enablePixy = 0;
            }
            else
            {
                enablePixy = 1;
            }
            break;
        default:
            updateValues(motorsState, type, value);
            break;
    }
}

void *naviThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint32_t type = 0, value = 0;
    MOTORS_DATA motorsState = {.setLeftSpeed = 0,
                               .setRightSpeed = 0,
                               .realLeftSpeed = 0,
                               .realRightSpeed = 0,
                               .measuredLeftSpeed = 0,
                               .measuredRightSpeed = 0,
                               .leftDir = 0,
                               .rightDir = 0,
                               .paused = 0};
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromNaviQ(&type, &value);
        naviEvent(&motorsState, type, value);
    }
}

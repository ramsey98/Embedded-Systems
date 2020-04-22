/*
 * PID.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include <navigation.h>
static int enablePID = 1, enableSensor = 1, enablePixy = 1, enableMovement = 1;
static int started = 0;

const naviLookupTable naviLookup[NAVILOOKUPLEN] = {{0, 25000},
                                                {40,18000},//{expected, measured}
                                                {60,10000},
                                                {80,7000},
                                                {100,5400},
                                                {120,4400},
                                                {140,3700},
                                                {160,3200},
                                                {180,2800},
                                                {200,2500},
                                                {220,2290},
                                                {240,2100},
                                                {255,1940}};

void updateMotors(MOTORS_STATE motorsState, MOTOR_DATA motor)
{
    if(motorsState.paused == 0)
    {
        if(motor.direction == 0)
        {
            if(motor.adjustedSpeed > 127)
            {
                sendMsgToUARTTxQ(motor.adjustedSpeed - 128, motor.forward8Bit);
            }
            else
            {
                sendMsgToUARTTxQ(motor.adjustedSpeed, motor.forward);
            }
        }
        else
        {
            if(motor.adjustedSpeed > 127)
            {
                sendMsgToUARTTxQ(motor.adjustedSpeed - 128, motor.reverse8Bit);
            }
            else
            {
                sendMsgToUARTTxQ(motor.adjustedSpeed, motor.reverse);
            }
        }
    }
    else if(motorsState.paused == 1)
    {
        sendMsgToUARTTxQ(0, motor.forward);
    }
}

void motorSet(MOTOR_DATA *motor, uint8_t value)
{
    if(value > MAX_SPEED)
    {
        motor->setSpeed = MAX_SPEED;
    }
    else
    {
        motor->setSpeed = value;
    }
}

void motorAccel(MOTOR_DATA *motor, uint8_t value)
{
    if(motor->setSpeed + value < MAX_SPEED)
    {
        motor->setSpeed += value;
    }
    else
    {
        motor->setSpeed = MAX_SPEED;
    }
}

void motorDecel(MOTOR_DATA *motor, uint8_t value)
{
    if(motor->setSpeed > value)
    {
        motor->setSpeed -= value;
    }
    else
    {
        motor->setSpeed = 0;
    }
}

void updateValues(MOTORS_STATE *motorsState, uint32_t type, uint32_t value)
{
    MQTTMsg pauseMsg = {.topic = JSON_TOPIC_STATE, .type = JSON_STATE_PAUSED, .value = 1};
    MQTTMsg resumeMsg = {.topic = JSON_TOPIC_STATE, .type = JSON_STATE_PAUSED, .value = 0};
    switch(type)
    {
        case PAUSE:
        {
            motorsState->paused = 1;
            sendMsgToMQTTQ(pauseMsg);
            break;
        }
        case RESUME:
        {
            motorsState->paused = 0;
            sendMsgToMQTTQ(resumeMsg);
            break;
        }
        case TURNLEFT:
        {
            motorsState->leftMotor.direction = 0;
            motorsState->rightMotor.direction = 0;
            motorDecel(&motorsState->leftMotor, value);
            motorAccel(&motorsState->rightMotor, value);
            break;
        }
        case TURNRIGHT:
        {
            motorsState->leftMotor.direction = 0;
            motorsState->rightMotor.direction = 0;
            motorAccel(&motorsState->leftMotor, value);
            motorDecel(&motorsState->rightMotor, value);
            break;
        }
        case FORWARD:
        {
            motorsState->leftMotor.direction = 0;
            motorsState->rightMotor.direction = 0;
            motorSet(&motorsState->leftMotor, value);
            motorSet(&motorsState->rightMotor, value);
            break;
        }
        case REVERSE:
        {
            motorsState->leftMotor.direction = 1;
            motorsState->rightMotor.direction = 1;
            motorSet(&motorsState->leftMotor, value);
            motorSet(&motorsState->rightMotor, value);
            break;
        }
        case ACCEL:
        {
            motorAccel(&motorsState->leftMotor, value);
            motorAccel(&motorsState->rightMotor, value);
            break;
        }
        case DECEL:
        {
            motorDecel(&motorsState->leftMotor, value);
            motorDecel(&motorsState->rightMotor, value);
            break;
        }
        default:
        {
            ERROR;
            break;
        }
    }
}

void PIDAdjust(MOTOR_DATA *motor)
{
    int i;
    static int error = 0, integral = 0;
    int PIDResult = 0;
    uint8_t realSpeed;
    if(motor->measuredSpeed == 0)
    {
        realSpeed = 0;
    }
    else
    {
        for(i = 0; i < NAVILOOKUPLEN; i++)
        {
            if(motor->measuredSpeed > naviLookup[i].measured)
            {
                realSpeed = naviLookup[i].expected;
                break;
            }
        }
    }
    integral = error;
    error = (int) motor->setSpeed - realSpeed;
    PIDResult = (KP*error) + (KI*integral);
    if(motor->adjustedSpeed + PIDResult < 0)
    {
        motor->adjustedSpeed = 0;
    }
    else if(motor->adjustedSpeed + PIDResult > MAX_SPEED)
    {
        motor->adjustedSpeed = MAX_SPEED;
    }
    else
    {
        motor->adjustedSpeed += PIDResult;
    }
    if(started == 1)
    {
        MQTTMsg msg = {.topic = JSON_TOPIC_DEBUG, .type = 0, .value = PIDResult};
        if(motor->motorID == 0)
        {
            msg.type = JSON_PID_LEFT;
        }
        else if(motor->motorID == 1)
        {
            msg.type = JSON_PID_RIGHT;
        }
        sendMsgToMQTTQ(msg);
    }
}

void naviEvent(MOTORS_STATE *motorsState, uint32_t type, uint32_t value)
{
    float diff, scaled;
    static MQTTMsg leftMsg = {.topic = JSON_TOPIC_DEBUG, .type = JSON_CAPTURE_LEFT, .value = 0};
    static MQTTMsg rightMsg = {.topic = JSON_TOPIC_DEBUG, .type = JSON_CAPTURE_RIGHT, .value = 0};
    switch(type)
    {
        case TIMER:
            if(enablePID != 0)
            {
                PIDAdjust(&motorsState->leftMotor);
                PIDAdjust(&motorsState->rightMotor);
            }
            else
            {
                motorsState->leftMotor.adjustedSpeed = motorsState->leftMotor.setSpeed;
                motorsState->rightMotor.adjustedSpeed = motorsState->rightMotor.setSpeed;
            }
            if(enableMovement != 0)
            {
                updateMotors(*motorsState, motorsState->leftMotor);
                updateMotors(*motorsState, motorsState->rightMotor);
            }
            leftMsg.value = motorsState->leftMotor.measuredSpeed;
            rightMsg.value = motorsState->rightMotor.measuredSpeed;
            if(started == 1)
            {
                sendMsgToMQTTQ(leftMsg);
                sendMsgToMQTTQ(rightMsg);
            }
            break;
        case LEFTCAP:
            motorsState->leftMotor.measuredSpeed = value;
            break;
        case RIGHTCAP:
            motorsState->rightMotor.measuredSpeed = value;
            break;
        case SENSOR:
            if(enableSensor != 0)
            {
                if(value >= 20)
                {
                    updateValues(motorsState, ACCEL, 20);
                }
                else if(value <= 12 & value >= 6)
                {
                    updateValues(motorsState, DECEL, 20);
                }
                else if(value < 6)
                {
                    updateValues(motorsState, FORWARD, 0);
                }
            }
            break;
        case PIXY:
            if(enablePixy != 0)
            {
                if(value < PIXY_X_LEFT)
                {
                    diff = PIXY_X_MIDDLE - value;
                    scaled = MAX_SPEED-(MAX_SPEED*(diff / PIXY_X_MIDDLE));
                    updateValues(motorsState, TURNLEFT, scaled);
                }
                else if(value > PIXY_X_RIGHT)
                {
                    diff = value - PIXY_X_MIDDLE;
                    scaled = MAX_SPEED*(diff / PIXY_X_MIDDLE);
                    updateValues(motorsState, TURNRIGHT, scaled);
                }
                else
                {
                    if(motorsState->leftMotor.setSpeed > motorsState->rightMotor.setSpeed)
                    {
                        updateValues(motorsState, FORWARD, motorsState->leftMotor.setSpeed);
                    }
                    else
                    {
                        updateValues(motorsState, FORWARD, motorsState->rightMotor.setSpeed);
                    }
                }
            }
            break;
        case PID_ENABLE:
            enablePID = value;
            break;
        case SENSOR_ENABLE:
            enableSensor = value;
            break;
        case PIXY_ENABLE:
            enablePixy = value;
            break;
        case MOVEMENT_ENABLE:
            enableMovement = value;
            break;
        case START:
            started = 1;
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
    MOTOR_DATA leftMotor = {.motorID = 0,
                            .direction = 0,
                             .setSpeed = 0,
                             .adjustedSpeed = 0,
                             .measuredSpeed = 0,
                             .forward = M0_FORWARD,
                             .forward8Bit = M0_FORWARD_8BIT,
                             .reverse = M0_REVERSE,
                             .reverse8Bit = M0_REVERSE_8BIT};
    MOTOR_DATA rightMotor = {.motorID = 1,
                             .direction = 0,
                             .setSpeed = 0,
                             .adjustedSpeed = 0,
                             .measuredSpeed = 0,
                             .forward = M1_FORWARD,
                             .forward8Bit = M1_FORWARD_8BIT,
                             .reverse = M1_REVERSE,
                             .reverse8Bit = M1_REVERSE_8BIT};
    MOTORS_STATE motorsState = {.paused = 0,
                                .leftMotor = leftMotor,
                                .rightMotor = rightMotor};
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromNaviQ(&type, &value);
        naviEvent(&motorsState, type, value);
    }
}

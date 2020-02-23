/*
 * motors_state.c
 *
 *  Created on: Feb 21, 2020
 *      Author: Holden Ramsey
 */

#include "motors_state.h"

int motors_fsm(MOTORS_DATA *motorsState, uint8_t type, uint8_t value)
{
    int success = 0;
    switch (motorsState->state)
    {
        case Motors_Init:
        {
            motorsState->leftSpeed = 0;
            motorsState->rightSpeed = 0;
            motorsState->leftDir = 0;
            motorsState->rightDir = 0;
            motorsState->state = Motors_Ready;
            break;
        }
        case Motors_Ready:
        {
            if(type == PAUSE)
            {
                motorsState->state = Motors_Paused;
            }
            else if(type == LEFTFORWARD)
            {
                motorsState->leftDir = 0;
                motorsState->leftSpeed = value;
            }
            else if(type == RIGHTFORWARD)
            {
                motorsState->rightDir = 0;
                motorsState->rightSpeed = value;
            }
            else if(type == LEFTREVERSE)
            {
                motorsState->leftDir = 1;
                motorsState->leftSpeed = value;
            }
            else if(type == RIGHTREVERSE)
            {
                motorsState->rightDir = 1;
                motorsState->rightSpeed = value;
            }
            else if(type == ACCEL)
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
            else if(type == DECEL)
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
            break;
        }
        case Motors_Paused:
        {
            if(type == RESUME)
            {
                motorsState->state = Motors_Ready;
            }
            break;
        }
        default:
        {
            success = -1;
            break;
        }
    }
    return success;
}



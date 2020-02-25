/*
 * capture_state.c
 *
 *  Created on: Feb 23, 2020
 *      Author: Holden Ramsey
 */

#include "capture_state.h"

int capture_fsm(CAPTURE_DATA *curState, uint32_t type, uint32_t period)
{
    int success = 0;
    switch (curState->state)
    {
        case Capture_Init:
        {
            curState->leftTotal = 0;
            curState->rightTotal = 0;
            curState->leftCount = 0;
            curState->rightCount = 0;
            curState->count = 0;
            curState->state = Capture_Polling;
            break;
        }
        case Capture_Polling:
        {
            if(type == LEFT)
            {
                curState->leftTotal += (SECOND/period);
                curState->leftCount++;
                GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
            }
            else if(type == RIGHT)
            {
                curState->rightTotal += (SECOND/period);
                curState->rightCount++;
            }
            curState->count++;
            if(curState->count == 100)
            {
                curState->state = Capture_Averaging;
            }
            break;
        }
        case Capture_Averaging:
        {
            curState->leftAvg = curState->leftTotal / curState->leftCount;
            curState->rightAvg = curState->rightTotal / curState->rightCount;
            uint8_t leftRPM = (curState->leftAvg/LINECOUNT);
            uint8_t rightRPM = (curState->rightAvg/LINECOUNT);
            success = sendLeftCapMsgToUARTDebugQ(leftRPM);
            success = sendRightCapMsgToUARTDebugQ(rightRPM);
            curState->leftTotal = 0;
            curState->rightTotal = 0;
            curState->leftCount = 0;
            curState->rightCount = 0;
            curState->count = 0;
            curState->state = Capture_Polling;
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




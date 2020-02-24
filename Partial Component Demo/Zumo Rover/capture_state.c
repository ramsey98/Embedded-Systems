/*
 * capture_state.c
 *
 *  Created on: Feb 23, 2020
 *      Author: Holden Ramsey
 */

#include "capture_state.h"

int capture_fsm(CAPTURE_DATA *curState, uint8_t type, uint8_t freq)
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
                curState->leftTotal += freq;
                curState->leftCount++;
            }
            else if(type == RIGHT)
            {
                curState->rightTotal += freq;
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
            success = sendLeftCapMsgToUARTDebugQ(curState->leftAvg);
            success = sendRightCapMsgToUARTDebugQ(curState->rightAvg);
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




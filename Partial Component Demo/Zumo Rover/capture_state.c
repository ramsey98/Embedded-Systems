/*
 * capture_state.c
 *
 *  Created on: Feb 23, 2020
 *      Author: Holden Ramsey
 */

#include "capture_state.h"

int capture_fsm(CAPTURE_DATA *curState, uint8_t leftFreq, uint8_t rightFreq)
{
    int success = 0;
    switch (curState->state)
    {
        case Capture_Init:
        {
            curState->leftFreq = 0;
            curState->rightFreq = 0;
            curState->count = 0;
            curState->state = Capture_Polling;
            break;
        }
        case Capture_Polling:
        {
            if(curState->leftFreq != leftFreq)
            {
                dbgUARTStr("Left Freq:");
                dbgUARTNum(leftFreq);
                curState->leftFreq = leftFreq;
            }
            if(curState->rightFreq != rightFreq)
            {
                dbgUARTStr("Right Freq:");
                dbgUARTNum(rightFreq);
                curState->rightFreq = rightFreq;
            }
            curState->state = Capture_Waiting;
            break;
        }
        case Capture_Waiting:
        {
            if(curState->count == 10)
            {
                curState->count = 0;
                curState->state = Capture_Polling;
            }
            else
            {
                curState->count++;
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




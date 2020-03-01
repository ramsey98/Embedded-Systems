/*
 * pixy_queue.c
 *
 *  Created on: Feb 22, 2020
 *      Author: Matthew
 */

#include "pixy_queue.h"
static QueueHandle_t xQueuePixy = NULL;

void createPixyQueue()
{
    xQueuePixy = xQueueCreate(16, sizeof(uint64_t));
    if(xQueuePixy == NULL)
    {
        halt();
    }
}

int sendTimeMsgToPixyQ1(unsigned int timeVal)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = TIMEFLAG | timeVal;
    success = xQueueSendFromISR(xQueuePixy, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendSpiMsgToPixyQ1() {
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = TRANSFERFLAG;
    success = xQueueSendFromISR(xQueuePixy, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromPixyQ1(int *timeInc, int *complete, uint64_t * block_data) {
   int ret = 0;
   BaseType_t success;
   dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
   uint64_t received;
   success = xQueueReceive(xQueuePixy, &received, portMAX_DELAY);

   if(success == pdFALSE)
   {
       ret = -1;
   }
   if (received >> SHIFT_PIXY == TIMEMASK)
   {
       *timeInc = received & FMASK;
   }

   if(received >> SHIFT_PIXY == TRANSFERMASK) {
       *complete = 1;
   }

   /* change to block data
   else if (received >> SHIFT_PIXY == SENSORMASK)
   {
       *sensorVal = received & FMASK;
       *timeInc = 0;
   } */
   dbgOutputLoc(AFTER_RECEIVE_QUEUE);
   return ret;
}



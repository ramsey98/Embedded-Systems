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
    dbgOutputLoc(SPI_BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = TIMEFLAG | timeVal;
    success = xQueueSendFromISR(xQueuePixy, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(SPI_AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromPixyQ1(int *timeVal, uint64_t * block_data) {
   int ret = 0;
   BaseType_t success;
   dbgOutputLoc(SPI_BEFORE_RECEIVE_QUEUE);
   uint64_t received;
   success = xQueueReceive(xQueuePixy, &received, portMAX_DELAY);

   if(success == pdFALSE)
   {
       ret = -1;
   }
   if (received >> SHIFT_PIXY == TIMEMASK)
   {
       dbgOutputLoc(TIME_FOUND);
       *timeVal = received & FMASK;
       //dbgUARTVal(*timeVal);
   }
   /* change to block data
   else if (received >> SHIFT_PIXY == SENSORMASK)
   {
       *sensorVal = received & FMASK;
       *timeInc = 0;
   } */
   dbgOutputLoc(SPI_AFTER_RECEIVE_QUEUE);
   return ret;
}



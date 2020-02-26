/*
 * pixy_queue.h
 *
 *  Created on: Feb 22, 2020
 *      Author: Matthew
 */

#ifndef PIXY_QUEUE_H_
#define PIXY_QUEUE_H_

#include "debug.h"
#include "queue.h"

#define FMASK (0xffffffff)
#define ZEROMASK (0x00000000)
#define TIMEMASK (0x00000001)
#define TIMEFLAG (0x0000000100000000)
#define PIXYBLOCKCOUNTMASK (0x10000000)
#define PIXYBLOCKMASK1 (0x00000010)
#define PIXYBLOCKMASK2 (0x00000011)
#define PIXYBLOCKMASK3 (0x00000100)

#define SHIFT_PIXY (32)

int sendTimeMsgToPixyQ1(unsigned int timeVal);
int sentBlockMsgToPixyQ1(uint8_t block_number, uint8_t data_block, uint8_t *data_bytes);
int receiveFromPixyQ1(int *timeVal, uint64_t * block_data);
void createPixyQueue();

#endif /* PIXY_QUEUE_H_ */

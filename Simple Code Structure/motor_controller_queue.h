/*
 * motor_controller_queue.h
 *
 *  Created on: Feb 24, 2020
 *      Author: jared
 */

#ifndef MOTOR_CONTROLLER_QUEUE_H_
#define MOTOR_CONTROLLER_QUEUE_H_

#include <FreeRTOS.h>
//#include <queue.h>
#include "debug.h"
#include "queue.h"

typedef struct {
    uint16_t move;
} MoveCommand;


static QueueHandle_t xQueue = NULL;

void createMotorControllerQueue();
int sendMsgToMCQ(uint16_t command);
uint16_t convertMovementCmd(uint8_t movementType, uint8_t magnitude);
int receiveMsgFromMCQueue(uint16_t * command);
int receiveMsgFromServer(MoveCommand* command, uint8_t movementType, uint8_t magnitude);

#endif /* MOTOR_CONTROLLER_QUEUE_H_ */

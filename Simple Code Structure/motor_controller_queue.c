/*
 * motor_controller_queue.c
 *
 *  Created on: Feb 24, 2020
 *      Author: jared
 */

#include "motor_controller_queue.h"

//static QueueHandle_t xQueue = NULL;

void createMotorControllerQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint16_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

int sendMsgToMCQ(uint16_t command) {
    MoveCommand msg;
    msg.move = command;

    int sent = xQueueSend(xQueue, (void *)&msg, portMAX_DELAY);

    return sent == SUCCESS;
}

uint16_t convertMovementCmd(uint8_t movement_type, uint8_t magnitude) {
    return ((uint16_t)movement_type << 8) | magnitude;
}

int receiveMsgFromMCQueue(uint16_t * command) {
    uint16_t msg;
    int receive = xQueueReceive(xQueue, &msg, portMAX_DELAY);
    if (receive == SUCCESS) {
        //Change state
        *command = msg;
        return SUCCESS;
    }

    return FAILURE;
}

int receiveMsgFromServer(MoveCommand* command, uint8_t movement_type, uint8_t magnitude) {

    // This should eventually receive commands from the server,
    // but I will hard code commands for the partial component

    uint16_t motor_command = convertMovementCmd(movement_type, magnitude);

    command->move = motor_command;

    return SUCCESS;
}

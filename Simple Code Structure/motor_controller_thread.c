/*
 * motor_controller_thread.c
 *
 *  Created on: Feb 24, 2020
 *      Author: jared
 */

#include "motor_controller_thread.h"

//static UART_Handle motors_uart;

void initWheels(MoveCommand * wheel_0, MoveCommand * wheel_1, MoveCommand * wheel_2) {
    uint16_t init = convertMovementCmd(STOP, STOP);

    wheel_0->move = init;
    wheel_1->move = init;
    wheel_2->move = init;
}

void *mcThread(void *arg0) {
    UART_Handle motors_uart;
    initialize(&motors_uart);

    while (1) {
        uint16_t command;
        int receive = receiveMsgFromMCQueue(&command);
        if (receive != SUCCESS) {
            ERROR;
        }

        handleMovementType(motors_uart, command);
    }
}

void initialize(UART_Handle * motors_uart) {

    UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 9600;
    uartParams.readEcho = UART_ECHO_OFF;
    *motors_uart = UART_open(CONFIG_UART_0, &uartParams);

    if (motors_uart == NULL) {
        ERROR;
    }

    sleep(2);
    uint8_t baudChar = 170;
    UART_write(*motors_uart, &baudChar, 1);

}

void handleMovementType(UART_Handle motors_uart, uint16_t command) {
    uint8_t movementType = (command >> 2) & 0x3F;

    switch (movementType) {
        case STOP:
            stopRover(motors_uart);
            break;
        case TURN:
            turnRover(motors_uart, command);
            break;
        case MOVE:
            moveRover(motors_uart, command);
            break;
        case SINGLE:
            moveSingle(motors_uart, command);
            break;
        default:
            GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
            break;
    }
}

void moveRover(UART_Handle motors_uart, uint16_t command) {
    uint8_t direction1 = command >> 15; // 0 for forward, 1 for backward
    uint8_t direction2 = direction1 ? 0 : 1;

    uint8_t magnitude = (command >> 8) & 0x7F;

    uint8_t address1 = (command & 0x3) + 128;
    uint8_t address2 = (address1 != 130) ? address1 + 1 : 128;

    moveWheel(motors_uart, address1, direction1, magnitude);
    moveWheel(motors_uart, address2, direction2, magnitude);
}

void moveSingle(UART_Handle motors_uart, uint16_t command) {
    uint8_t direction = command >> 15; // 0 for left, 1 for right
    uint8_t magnitude = (command >> 8) & 0x7F;
    uint8_t address = (command & 0x3) + 128;

    moveWheel(motors_uart, address, direction, magnitude);
}

void moveWheel(UART_Handle motors_uart, uint8_t address, uint8_t direction, uint8_t magnitude) {
    uint8_t checksum = (address + direction + magnitude) & 0b01111111;
    uint8_t buffer[] = {address, direction, magnitude, checksum};
    UART_write(motors_uart, buffer, sizeof(buffer));
}

void stopRover(UART_Handle motors_uart) {
    uint8_t magnitude = 0;
    uint8_t direction = 0;

    moveAllMotorsSame(motors_uart, magnitude, direction);
}

void turnRover(UART_Handle motors_uart, uint16_t command) {
    uint8_t direction = command >> 15; // 0 for forward, 1 for backward
    uint8_t magnitude = (command >> 8) & 0x7F;

    moveAllMotorsSame(motors_uart, magnitude, direction);
}

void moveAllMotorsSame(UART_Handle motors_uart, uint8_t magnitude, uint8_t direction) {
    uint8_t address;

    for (address = 128; address <= 130; address++) {
        moveWheel(motors_uart, address, direction, magnitude);
    }
}

//void *UARTTxThread(void *arg0)
//{
//    dbgOutputLoc(ENTER_TASK);
//    uint8_t byte1;
//    uint16_t value;
//    int received = 0;
//    dbgOutputLoc(WHILE1);
//    while(1)
//    {
//        received = receiveFromUARTTxQ(&value);
//        if(received == -1)
//        {
//            ERROR;
//        }
//        else
//        {
//            byte1 = value & 0xFF;
//            if(byte1 == 0)
//            {
//                byte1 = value >> 8;
//                UART_write(motors_uart, &byte1, sizeof(byte1));
//            }
//            else
//            {
//                UART_write(motors_uart, &value, sizeof(value));
//            }
//        }
//    }
//}
//
//void *UARTDebugThread(void *arg0)
//{
//    dbgOutputLoc(ENTER_TASK);
//    uint8_t value, type;
//    int received = 0;
//    dbgOutputLoc(WHILE1);
//    while(1)
//    {
//        received = receiveFromUARTDebugQ(&type, &value);
//        if(received == -1)
//        {
//            ERROR;
//        }
//        else
//        {
//            if(type == 1)
//            {
//                dbgUARTStr("Left Motor:");
//            }
//            else if(type == 2)
//            {
//                dbgUARTStr("Right Motor:");
//            }
//            dbgUARTNum(value);
//        }
//    }
//}

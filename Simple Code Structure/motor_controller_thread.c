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
    GPIO_toggle(CONFIG_LED_1_GPIO);
    sleep(1);
//    initialize();
    GPIO_toggle(CONFIG_LED_0_GPIO);
    sleep(1);
    GPIO_toggle(CONFIG_LED_1_GPIO);
    sleep(1);
    GPIO_toggle(CONFIG_LED_0_GPIO);
    sleep(1);
    GPIO_toggle(CONFIG_LED_1_GPIO);
    sleep(1);

//    while (1) {
//        uint16_t command;
//        int receive = receiveMsgFromMCQueue(&command);
//        if (receive != SUCCESS) {
//            ERROR;
//        }
//
//        moveRover(command);
//    }
//    while(1) {
//
//    }
    return(NULL);
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

void moveRover(uint16_t command) {
    UART_Handle motors_uart;
    initialize(&motors_uart);

    uint8_t direction = 0;
    uint8_t magnitude = 64;

    uint8_t address = 128;
    uint8_t checksum28 = (address + direction + magnitude) & 0b01111111;
    uint8_t buffer28[] = {address, direction, magnitude, checksum28};

    address = 129;
    uint8_t checksum29 = (address + direction + magnitude) & 0b01111111;
    uint8_t buffer29[] = {address, direction, magnitude, checksum29};

    address = 130;
    uint8_t checksum30 = (address + direction + magnitude) & 0b01111111;
    uint8_t buffer30[] = {address, direction, magnitude, checksum30};

    UART_write(motors_uart, buffer28, sizeof(buffer28));
    UART_write(motors_uart, buffer29, sizeof(buffer29));
    UART_write(motors_uart, buffer30, sizeof(buffer30));

    sleep(2);

    magnitude = 0;

    address = 128;
    uint8_t checksum128 = (address + direction + magnitude) & 0b01111111;
    uint8_t buffer128[] = {address, direction, magnitude, checksum128};

    address = 129;
    uint8_t checksum129 = (address + direction + magnitude) & 0b01111111;
    uint8_t buffer129[] = {address, direction, magnitude, checksum129};

    address = 130;
    uint8_t checksum130 = (address + direction + magnitude) & 0b01111111;
    uint8_t buffer130[] = {address, direction, magnitude, checksum130};

    UART_write(motors_uart, buffer128, sizeof(buffer128));
    UART_write(motors_uart, buffer129, sizeof(buffer129));
    UART_write(motors_uart, buffer130, sizeof(buffer130));

    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
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

/*
 * motor_controller_thread.c
 *
 *  Created on: Feb 24, 2020
 *      Author: jared
 */

#include "motor_controller_thread.h"

void initWheels(MoveCommand * wheel_0, MoveCommand * wheel_1, MoveCommand * wheel_2) {
    uint16_t init = convertMovementCmd(STOP, STOP);

    wheel_0->move = init;
    wheel_1->move = init;
    wheel_2->move = init;
}

void *mcThread(void *arg0) {
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
    sleep(1);
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
    sleep(1);
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
    sleep(1);
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
    sleep(1);
//    MoveCommand wheel_0;
//    MoveCommand wheel_1;
//    MoveCommand wheel_2;
//    UART_Handle uart;
//
//    initialize(&uart);
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
    sleep(1);

    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);
//    initWheels(&wheel_0, &wheel_1, &wheel_2);
    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
    sleep(1);


    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);
    sleep(3);
    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
    sleep(1);

//    while (1) {
//        uint16_t command;
//        int receive = receiveMsgFromMCQueue(&command);
//        if (receive != SUCCESS) {
//            ERROR;
//        }
//
//        moveRover(uart, command);
//    }
    while(1) {

    }
}

void initialize(UART_Handle * uart) {
    UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 9600;
    uartParams.readEcho = UART_ECHO_OFF;
    *uart = UART_open(0, &uartParams);

    if (uart == NULL) {
        ERROR;
    }

    // sent baud rate
    sleep(2);
    uint8_t baudChar = 170;
    UART_write(*uart, &baudChar, 1);

}

void moveRover(UART_Handle uart, uint16_t command) {
    uint8_t direction = command >> 8;
    uint8_t magnitude = command & 0xFF;
    uint8_t checksum = (0x80 + 0x04 + 0x0F) & 0b01111111;

    uint8_t buffer[] = {0x80, direction, magnitude, checksum};
    UART_write(uart, buffer, sizeof(buffer));
}


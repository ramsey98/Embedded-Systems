/*
 * UARTTx_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include <motors.h>

static QueueHandle_t xQueue = NULL;
static UART_Handle motors_uart;

void motorsUARTInit()
{
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 38400;
    uartParams.readEcho = UART_ECHO_OFF;
    motors_uart = UART_open(CONFIG_UART_1, &uartParams);
    if (motors_uart == NULL) ERROR;
    sendMsgToUARTTxQ(INIT_CONTROLLER, EMPTY);
}

void *UARTTxThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint8_t byte1;
    uint16_t value;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromUARTTxQ(&value);
        byte1 = value;
        if(byte1 == EMPTY)
        {
            byte1 = value >> 8;
            UART_write(motors_uart, &byte1, sizeof(byte1));
        }
        else
        {
            UART_write(motors_uart, &value, sizeof(value));
        }
    }
}

void createUARTTxQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint16_t));
    if(xQueue == NULL) ERROR;
}

void sendMsgToUARTTxQ(uint8_t byte1, uint8_t byte2)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    uint16_t msg = (byte1 << 8) | byte2;
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromUARTTxQ(uint16_t * value)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint16_t received;
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *value = received;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}

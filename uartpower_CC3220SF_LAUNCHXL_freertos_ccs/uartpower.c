/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== uartpower.c ========
 */
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define NLOOPS 200

static void blinkLED(uint32_t led, uint32_t count);

const char dummyBuffer[] = "abcdefghijklmnopqrstuvwxyz\r\n";
const char sleepMsg[] = "\fGoing to sleep for 5 seconds...\r\n";

/*
 *  ======== blinkLED ========
 */
static void blinkLED(uint32_t led, uint32_t count)
{
    int i;

    for (i = 0; i < 500000 * (count * 2); i++) {
        if ((i % 500000) == 0) {
            GPIO_toggle(led);
        }
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    UART_Handle uart0;
    UART_Params uartParams;
    int         i;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /*
     *  This function enables the Power policy function to run
     *  in the idle loop. Alternatively, you can set the field
     *  'enablePolicy' in the Power_config structure ("BOARDNAME".c)
     *  to 1.
     */
    Power_enablePolicy();

    /* Create a UART with the parameters below. */
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.baudRate = 115200;
    uartParams.readEcho = UART_ECHO_OFF;

    uart0 = UART_open(CONFIG_UART_0, &uartParams);
    if (uart0 == NULL) {
        /* Error creating UART */
        while (1);
    }

    /* Stop the receive to go into low power state */
    UART_control(uart0, UART_CMD_RXDISABLE, NULL);

    /* Write to UART0 */
    while (1) {
        for (i = 0; i < NLOOPS; i++) {
            UART_write(uart0, (const void *)dummyBuffer, sizeof(dummyBuffer));
        }

        UART_write(uart0, (const void *)sleepMsg, sizeof(sleepMsg));

        /* Sleep for 5 seconds */
        sleep(5);

        /* Blink the LED to indicate that we woke up. */
        blinkLED(CONFIG_GPIO_LED_0, 2);
    }
}

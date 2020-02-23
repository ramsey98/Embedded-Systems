/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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

/*****************************************************************************

   Application Name     -   MQTT Client
   Application Overview -   The device is running a MQTT client which is
                           connected to the online broker. Three LEDs on the
                           device can be controlled from a web client by
                           publishing msg on appropriate topics. Similarly,
                           message can be published on pre-configured topics
                           by pressing the switch buttons on the device.

   Application Details  - Refer to 'MQTT Client' README.html

*****************************************************************************/
//*****************************************************************************
//
//! \addtogroup mqtt_server
//! @{
//
//*****************************************************************************
#include <mqtt_client.h>

//*****************************************************************************
//                 GLOBAL VARIABLES
//*****************************************************************************

/* Connection state: (0) - connected, (negative) - disconnected              */
int32_t gApConnectionState = -1;
uint32_t memPtrCounterfree = 0;
static MQTTClient_Handle gMqttClient;
MQTTClient_Params MqttClientExmple_params;
unsigned short g_usTimerInts;

/* Receive task handle                                                       */
pthread_t g_rx_task_hndl = (pthread_t) NULL;
uint32_t gUiConnFlag = 0;

/* AP Security Parameters                                                    */
SlWlanSecParams_t SecurityParams = { 0 };

/* Client ID                                                                 */
/* If ClientId isn't set, the MAC address of the device will be copied into  */
/* the ClientID parameter.                                                   */
char ClientId[13] = {'\0'};

/* Client User Name and Password                                             */
const char *ClientUsername = "username1";
const char *ClientPassword = "pwd1";

/* Subscription topics and qos values                                        */
char *topic[SUBSCRIPTION_TOPIC_COUNT] =
{
  SUBSCRIPTION_TOPIC0
};

//https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels/
unsigned char qos[SUBSCRIPTION_TOPIC_COUNT] =
{ MQTT_QOS_2 };


/* Message Queue                                                             */
pthread_t mqttThread = (pthread_t) NULL;
pthread_t appThread = (pthread_t) NULL;
timer_t g_timer;

/* Printing new line                                                         */
char lineBreak[] = "\n\r";

//*****************************************************************************
//                 Banner VARIABLES
//*****************************************************************************
#ifdef  SECURE_CLIENT

char *Mqtt_Client_secure_files[CLIENT_NUM_SECURE_FILES] = {"ca-cert.pem"};

/*Initialization structure to be used with sl_ExtMqtt_Init API. In order to  */
/*use secured socket method, the flag MQTTCLIENT_NETCONN_SEC, cipher,        */
/*n_files and secure_files must be configured.                               */
/*certificates also must be programmed  ("ca-cert.pem").                     */
/*The first parameter is a bit mask which configures server address type and */
/*security mode.                                                             */
/*Server address type: IPv4, IPv6 and URL must be declared with The          */
/*corresponding flag.                                                        */
/*Security mode: The flag MQTTCLIENT_NETCONN_SEC enables the security (TLS)  */
/*which includes domain name verification and certificate catalog            */
/*verification, those verifications can be disabled by adding to the bit mask*/
/*MQTTCLIENT_NETCONN_SKIP_DOMAIN_NAME_VERIFICATION and                       */
/*MQTTCLIENT_NETCONN_SKIP_CERTIFICATE_CATALOG_VERIFICATION flags             */
/*Example: MQTTCLIENT_NETCONN_IP6 | MQTTCLIENT_NETCONN_SEC |                 */
/*MQTTCLIENT_NETCONN_SKIP_CERTIFICATE_CATALOG_VERIFICATION                   */
/*For this bit mask, the IPv6 address type will be in use, the security      */
/*feature will be enable and the certificate catalog verification will be    */
/*skipped.                                                                   */
/*Note: The domain name verification requires URL Server address type        */
/*      otherwise, this verification will be disabled.                       */
MQTTClient_ConnParams Mqtt_ClientCtx =
{
    MQTTCLIENT_NETCONN_IP4 | MQTTCLIENT_NETCONN_SEC,
    SERVER_IP_ADDRESS,  //SERVER_ADDRESS,
    SECURED_PORT_NUMBER, //  PORT_NUMBER
    SLNETSOCK_SEC_METHOD_SSLv3_TLSV1_2,
    SLNETSOCK_SEC_CIPHER_FULL_LIST,
    CLIENT_NUM_SECURE_FILES,
    Mqtt_Client_secure_files
};

void setTime()
{
    SlDateTime_t dateTime = {0};
    dateTime.tm_day = (uint32_t)DAY;
    dateTime.tm_mon = (uint32_t)MONTH;
    dateTime.tm_year = (uint32_t)YEAR;
    dateTime.tm_hour = (uint32_t)HOUR;
    dateTime.tm_min = (uint32_t)MINUTES;
    dateTime.tm_sec = (uint32_t)SEC;
    sl_DeviceSet(SL_DEVICE_GENERAL, SL_DEVICE_GENERAL_DATE_TIME,
                 sizeof(SlDateTime_t), (uint8_t *)(&dateTime));
}

#else
MQTTClient_ConnParams Mqtt_ClientCtx =
{
    MQTTCLIENT_NETCONN_URL,
    SERVER_ADDRESS,
    PORT_NUMBER, 0, 0, 0,
    NULL
};
#endif

/* Initialize the will_param structure to the default will parameters        */
MQTTClient_Will will_param =
{
    WILL_TOPIC,
    WILL_MSG,
    WILL_QOS,
    WILL_RETAIN
};

//*****************************************************************************
//
//! Periodic Timer Interrupt Handler
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void TimerPeriodicIntHandler(sigval val)
{
    /* Increment our interrupt counter.                                      */
    g_usTimerInts++;

    if(!(g_usTimerInts & 0x1))
    {
        /* Turn Led Off                                                      */
        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
    }
    else
    {
        /* Turn Led On                                                       */
        GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
    }
}

//*****************************************************************************
//
//! Function to configure and start timer to blink the LED while device is
//! trying to connect to an AP
//!
//! \param none
//!
//! return none
//
//*****************************************************************************
void LedTimerConfigNStart()
{
    struct itimerspec value;
    sigevent sev;

    /* Create Timer                                                          */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_notify_function = &TimerPeriodicIntHandler;
    timer_create(2, &sev, &g_timer);

    /* start timer                                                           */
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = TIMER_EXPIRATION_VALUE;
    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = TIMER_EXPIRATION_VALUE;

    timer_settime(g_timer, 0, &value, NULL);
}

//*****************************************************************************
//
//! Disable the LED blinking Timer as Device is connected to AP
//!
//! \param none
//!
//! return none
//
//*****************************************************************************
void LedTimerDeinitStop()
{
    /* Disable the LED blinking Timer as Device is connected to AP.          */
    timer_delete(g_timer);
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************

void * MqttClientThread(void * pvParameters)
{
    MQTTClient_run((MQTTClient_Handle)pvParameters);
    pthread_exit(0);
    return(NULL);
}

//*****************************************************************************
//
//! Task implementing MQTT Server plus client bridge
//!
//! This function
//!    1. Initializes network driver and connects to the default AP
//!    2. Initializes the mqtt client ans server libraries and set up MQTT
//!       with the remote broker.
//!    3. set up the button events and their callbacks(for publishing)
//!    4. handles the callback signals
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************
void * MqttClient(void *pvParameters)
{
    long lRetVal = -1;

    /*Initializing Client and Subscribing to the Broker.                     */
    if(gApConnectionState >= 0)
    {
        lRetVal = MqttClient_start();
        if(lRetVal == -1)
        {
            UART_PRINT("MQTT Client lib initialization failed\n\r");
            ERROR;
            //pthread_exit(0);
            //return(NULL);
        }
    }

    /*handling the signals from various callbacks including the push button  */
    /*prompting the client to publish a msg on PUB_TOPIC OR msg received by  */
    /*the server on enrolled topic(for which the on-board client ha enrolled)*/
    /*from a local client(will be published to the remote broker by the      */
    /*client) OR msg received by the client from the remote broker (need to  */
    /*be sent to the server to see if any local client has subscribed on the */
    /*same topic).                                                           */
    char payload[PUBLISH_JSON_BUFFER_SIZE] = {0};
    char publish_topic[PUBLISH_TOPIC_BUFFER_SIZE] = {0};
    uint8_t msgType, state, leftmotor, rightmotor;
    while(1)
    {
        receiveFromMQTTQ(&msgType, &state, &leftmotor, &rightmotor);
        memset(payload, 0, PUBLISH_JSON_BUFFER_SIZE);
        json_write(payload, msgType, state, leftmotor, rightmotor);
        switch(msgType)
        {
        case MQTT_STATE:
            strncpy(publish_topic, PUBLISH_TOPIC0, sizeof(PUBLISH_TOPIC0));
            break;
        case MQTT_LEFTMOTOR:
            strncpy(publish_topic, PUBLISH_TOPIC1, sizeof(PUBLISH_TOPIC1));
            break;
        case MQTT_RIGHTMOTOR:
            strncpy(publish_topic, PUBLISH_TOPIC2, sizeof(PUBLISH_TOPIC2));
            break;
        default:
            ERROR;
            break;
        }
        lRetVal = MQTTClient_publish
        (
            gMqttClient, (char*) publish_topic,
            strlen((char*) publish_topic), payload,
            strlen((char*) payload),
            MQTT_QOS_2 | ((RETAIN_ENABLE) ? MQTT_PUBLISH_RETAIN : 0)
        );
    }
}

void DisplayBanner(char * AppName)
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t    CC32xx %s Application       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}

//*****************************************************************************
//
//! This function connect the MQTT device to an AP with the SSID which was
//! configured in SSID_NAME definition which can be found in Network_if.h file,
//! if the device can't connect to to this AP a request from the user for other
//! SSID will appear.
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************
int32_t Mqtt_IF_Connect()
{
    int32_t lRetVal;
    char SSID_Remote_Name[32];
    int8_t Str_Length;

    memset(SSID_Remote_Name, '\0', sizeof(SSID_Remote_Name));
    Str_Length = strlen(SSID_NAME);

    if(Str_Length)
    {
        /*Copy the Default SSID to the local variable                        */
        strncpy(SSID_Remote_Name, SSID_NAME, Str_Length);
    }

    /*Display Application Banner                                             */
    DisplayBanner(APPLICATION_NAME);

    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);

    /*Reset The state of the machine                                         */
    Network_IF_ResetMCUStateMachine();

    /*Start the driver                                                       */
    lRetVal = Network_IF_InitDriver(ROLE_STA);
    if(lRetVal < 0)
    {
        UART_PRINT("Failed to start SimpleLink Device\n\r", lRetVal);
        ERROR;
        return(-1);
    }

    /*switch on CONFIG_GPIO_LED_2 to indicate Simplelink is properly up.       */
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);

    /*Start Timer to blink CONFIG_GPIO_LED_0 till AP connection                */
    LedTimerConfigNStart();

    /*Initialize AP security params                                          */
    SecurityParams.Key = (signed char *) SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type = SECURITY_TYPE;
    /*Connect to the Access Point                                            */
    lRetVal = Network_IF_ConnectAP(SSID_Remote_Name, SecurityParams);
    if(lRetVal < 0)
    {
        UART_PRINT("Connection to an AP failed\n\r");
        ERROR;
        return(-1);
    }
    /*Disable the LED blinking Timer as Device is connected to AP.           */
    LedTimerDeinitStop();

    /*Switch ON CONFIG_GPIO_LED_0 to indicate that Device acquired an IP.      */
    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);

    sleep(1);

    GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);

    return(0);
}

//*****************************************************************************
//!
//! MQTT Start - Initialize and create all the items required to run the MQTT
//! protocol
//!
//! \param  none
//!
//! \return None
//!
//*****************************************************************************
void Mqtt_start()
{
    int32_t threadArg = 100;
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int32_t retc = 0;

    /*Set priority and stack size attributes                                 */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, MQTTTHREADSIZE);
    retc |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);

    if(retc != 0)
    {
        UART_PRINT("MQTT thread create fail\n\r");
        ERROR;
        return;
    }

    retc = pthread_create(&mqttThread, &pAttrs, MqttClient, (void *) &threadArg);
    if(retc != 0)
    {
        UART_PRINT("MQTT thread create fail\n\r");
        ERROR;
        return;
    }
}

int32_t MqttClient_start()
{
    int32_t lRetVal = -1;
    int32_t iCount = 0;

    int32_t threadArg = 100;
    pthread_attr_t pAttrs;
    struct sched_param priParam;

    MqttClientExmple_params.clientId = ClientId;
    MqttClientExmple_params.connParams = &Mqtt_ClientCtx;
    MqttClientExmple_params.mqttMode31 = MQTT_3_1;
    MqttClientExmple_params.blockingSend = true;

    /*Initialize MQTT client lib                                             */
    gMqttClient = MQTTClient_create(MqttClientCallback,
                                    &MqttClientExmple_params);
    if(gMqttClient == NULL)
    {
        /*lib initialization failed                                          */
        ERROR;
        return(-1);
    }

    /*Open Client Receive Thread start the receive task. Set priority and    */
    /*stack size attributes                                                  */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;
    lRetVal = pthread_attr_setschedparam(&pAttrs, &priParam);
    lRetVal |= pthread_attr_setstacksize(&pAttrs, RXTASKSIZE);
    lRetVal |= pthread_attr_setdetachstate(&pAttrs, PTHREAD_CREATE_DETACHED);
    lRetVal |=
        pthread_create(&g_rx_task_hndl, &pAttrs, MqttClientThread,
                       (void *) &threadArg);
    if(lRetVal != 0)
    {
        UART_PRINT("Client Thread Create Failed failed\n\r");
        ERROR;
        return(-1);
    }
#ifdef SECURE_CLIENT
    setTime();
#endif

    /*setting will parameters                                                */
    MQTTClient_set(gMqttClient, MQTTClient_WILL_PARAM, &will_param,
                   sizeof(will_param));

#ifdef CLNT_USR_PWD
    /*Set user name for client connection                                    */
    MQTTClient_set(gMqttClient, MQTTClient_USER_NAME, (void *)ClientUsername,
                   strlen(
                       (char*)ClientUsername));

    /*Set password                                                           */
    MQTTClient_set(gMqttClient, MQTTClient_PASSWORD, (void *)ClientPassword,
                   strlen(
                       (char*)ClientPassword));
#endif
    /*Initiate MQTT Connect                                                  */
    if(gApConnectionState >= 0)
    {
#if CLEAN_SESSION == false
        bool clean = CLEAN_SESSION;
        MQTTClient_set(gMqttClient, MQTTClient_CLEAN_CONNECT, (void *)&clean,
                       sizeof(bool));
#endif
        /*The return code of MQTTClient_connect is the ConnACK value that
           returns from the server */
        lRetVal = MQTTClient_connect(gMqttClient);

        /*negative lRetVal means error,
           0 means connection successful without session stored by the server,
           greater than 0 means successful connection with session stored by
           the server */
        if(0 > lRetVal)
        {
            /*lib initialization failed                                      */
            UART_PRINT("Connection to broker failed, Error code: %d\n\r",lRetVal);
            gUiConnFlag = 0;
        }
        else
        {
            gUiConnFlag = 1;
        }
        /*Subscribe to topics when session is not stored by the server       */
        if((gUiConnFlag == 1) && (0 == lRetVal))
        {
            uint8_t subIndex;
            MQTTClient_SubscribeParams subscriptionInfo[
                SUBSCRIPTION_TOPIC_COUNT];

            for(subIndex = 0; subIndex < SUBSCRIPTION_TOPIC_COUNT; subIndex++)
            {
                subscriptionInfo[subIndex].topic = topic[subIndex];
                subscriptionInfo[subIndex].qos = qos[subIndex];
            }

            if(MQTTClient_subscribe(gMqttClient, subscriptionInfo,
                                    SUBSCRIPTION_TOPIC_COUNT) < 0)
            {
                UART_PRINT("\n\r Subscription Error \n\r");
                MQTTClient_disconnect(gMqttClient);
                gUiConnFlag = 0;
            }
            else
            {
                for(iCount = 0; iCount < SUBSCRIPTION_TOPIC_COUNT; iCount++)
                {
                    UART_PRINT("Client subscribed on %s\n\r,", topic[iCount]);
                }
            }
        }
    }

    return(0);
}

//*****************************************************************************
//!
//! Set the ClientId with its own mac address
//! This routine converts the mac address which is given
//! by an integer type variable in hexadecimal base to ASCII
//! representation, and copies it into the ClientId parameter.
//!
//! \param  macAddress  -   Points to string Hex.
//!
//! \return void.
//!
//*****************************************************************************
int32_t SetClientIdNamefromMacAddress()
{
    int32_t ret = 0;
    uint8_t Client_Mac_Name[2];
    uint8_t Index;
    uint16_t macAddressLen = SL_MAC_ADDR_LEN;
    uint8_t macAddress[SL_MAC_ADDR_LEN];

    /*Get the device Mac address */
    ret = sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                       &macAddress[0]);

    /*When ClientID isn't set, use the mac address as ClientID               */
    if(ClientId[0] == '\0')
    {
        /*6 bytes is the length of the mac address                           */
        for(Index = 0; Index < SL_MAC_ADDR_LEN; Index++)
        {
            /*Each mac address byte contains two hexadecimal characters      */
            /*Copy the 4 MSB - the most significant character                */
            Client_Mac_Name[0] = (macAddress[Index] >> 4) & 0xf;
            /*Copy the 4 LSB - the least significant character               */
            Client_Mac_Name[1] = macAddress[Index] & 0xf;

            if(Client_Mac_Name[0] > 9)
            {
                /*Converts and copies from number that is greater than 9 in  */
                /*hexadecimal representation (a to f) into ascii character   */
                ClientId[2 * Index] = Client_Mac_Name[0] + 'a' - 10;
            }
            else
            {
                /*Converts and copies from number 0 - 9 in hexadecimal       */
                /*representation into ascii character                        */
                ClientId[2 * Index] = Client_Mac_Name[0] + '0';
            }
            if(Client_Mac_Name[1] > 9)
            {
                /*Converts and copies from number that is greater than 9 in  */
                /*hexadecimal representation (a to f) into ascii character   */
                ClientId[2 * Index + 1] = Client_Mac_Name[1] + 'a' - 10;
            }
            else
            {
                /*Converts and copies from number 0 - 9 in hexadecimal       */
                /*representation into ascii character                        */
                ClientId[2 * Index + 1] = Client_Mac_Name[1] + '0';
            }
        }
    }
    return(ret);
}

void printBorder(char ch,
                 int n)
{
    int i = 0;

    for(i = 0; i < n; i++)
    {
        putch(ch);
    }
}

int32_t DisplayAppBanner(char* appName,
                         char* appVersion)
{
    int32_t ret = 0;
    uint8_t macAddress[SL_MAC_ADDR_LEN];
    uint16_t macAddressLen = SL_MAC_ADDR_LEN;
    uint16_t ConfigSize = 0;
    uint8_t ConfigOpt = SL_DEVICE_GENERAL_VERSION;
    SlDeviceVersion_t ver = {0};
    /* Printing new line                                                         */
    char lineBreak[] = "\n\r";

    ConfigSize = sizeof(SlDeviceVersion_t);

    /*Print device version info. */
    ret =
        sl_DeviceGet(SL_DEVICE_GENERAL, &ConfigOpt, &ConfigSize,
                     (uint8_t*)(&ver));

    /*Print device Mac address */
    ret |= (int32_t)sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                       &macAddress[0]);

    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 44);
    UART_PRINT(lineBreak);
    UART_PRINT("\t   %s Example Ver: %s",appName, appVersion);
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 44);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);
    UART_PRINT("\t CHIP: 0x%x",ver.ChipId);
    UART_PRINT(lineBreak);
    UART_PRINT("\t MAC:  %d.%d.%d.%d",ver.FwVersion[0],ver.FwVersion[1],
               ver.FwVersion[2],
               ver.FwVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t PHY:  %d.%d.%d.%d",ver.PhyVersion[0],ver.PhyVersion[1],
               ver.PhyVersion[2],
               ver.PhyVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t NWP:  %d.%d.%d.%d",ver.NwpVersion[0],ver.NwpVersion[1],
               ver.NwpVersion[2],
               ver.NwpVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t ROM:  %d",ver.RomVersion);
    UART_PRINT(lineBreak);
    UART_PRINT("\t HOST: %s", SL_DRIVER_VERSION);
    UART_PRINT(lineBreak);
    UART_PRINT("\t MAC address: %02x:%02x:%02x:%02x:%02x:%02x", macAddress[0],
               macAddress[1], macAddress[2], macAddress[3], macAddress[4],
               macAddress[5]);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 44);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);

    return(ret);
}

//*****************************************************************************
//!
//! Utility function which Display the app banner
//!
//! \param[in] appName     -  holds the application name.
//! \param[in] appVersion  -  holds the application version.
//!
//! \return none.
//!
//*****************************************************************************

void *commThread(void * args)
{
    pthread_t spawn_thread = (pthread_t) NULL;
    pthread_attr_t pAttrs_spawn;
    struct sched_param priParam;
    int32_t retc = 0;
    UART_Handle tUartHndl;
    /*Initialize SlNetSock layer with CC31xx/CC32xx interface */
    SlNetIf_init(0);
    SlNetIf_add(SLNETIF_ID_1, "CC32xx",
                (const SlNetIf_Config_t *)&SlNetIfConfigWifi,
                SLNET_IF_WIFI_PRIO);
    SlNetSock_init(0);
    SlNetUtil_init(0);

    /*Configure the UART                                                     */
    tUartHndl = InitTerm();
    /*remove uart receive from LPDS dependency                               */
    UART_control(tUartHndl, UART_CMD_RXDISABLE, NULL);
    /*Create the sl_Task                                                     */
    pthread_attr_init(&pAttrs_spawn);
    priParam.sched_priority = SPAWN_TASK_PRIORITY;
    retc = pthread_attr_setschedparam(&pAttrs_spawn, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs_spawn, TASKSTACKSIZE);
    retc |= pthread_attr_setdetachstate(&pAttrs_spawn, PTHREAD_CREATE_DETACHED);
    retc = pthread_create(&spawn_thread, &pAttrs_spawn, sl_Task, NULL);
    if(retc != 0)
    {
        ERROR; //could not create simplelink task
    }

    retc = sl_Start(0, 0, 0);
    if(retc < 0)
    {
        ERROR; //sl_Start failed
    }
    /*Output device information to the UART terminal */
    retc = DisplayAppBanner(APPLICATION_NAME, APPLICATION_VERSION);
    /*Set the ClientId with its own mac address */
    retc |= SetClientIdNamefromMacAddress();
    retc = sl_Stop(SL_STOP_TIMEOUT);
    if(retc < 0)
    {
        ERROR; //sl_Stop failed
    }
    if(retc < 0)
    {
        ERROR; //mqtt_client - Unable to retrieve device information
    }

    /*Connect to AP                                                      */
    gApConnectionState = Mqtt_IF_Connect();
    /*Run MQTT Main Thread (it will open the Client and Server)          */
    Mqtt_start();

    if (gApConnectionState >= 0)
    {
        Mqtt_start();
    }
    return NULL;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

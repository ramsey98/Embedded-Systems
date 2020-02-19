/*
 * mqtt_client.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

/* Standard includes                                                         */
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <time.h>
#include <unistd.h>

/* TI-Driver includes                                                        */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Simplelink includes                                                       */
#include <ti/drivers/net/wifi/simplelink.h>

/* SlNetSock includes                                                        */
#include <ti/drivers/net/wifi/slnetifwifi.h>

/* MQTT Library includes                                                     */
#include <ti/net/mqtt/mqttclient.h>

/* Common interface includes                                                 */
#include "network_if.h"
#include "uart_term.h"

/* TI-DRIVERS Header files */
#include "ti_drivers_config.h"

/* Application includes                                                      */
#include "client_cbs.h"

// Embedded includes
#include "debug.h"
#include "mqtt_queue.h"
#include "json_parse.h"
//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
/* enables secured client                                                    */
//#define SECURE_CLIENT

/* enables client authentication by the server                               */
//#define CLNT_USR_PWD

#define CLIENT_INIT_STATE        (0x01)
#define MQTT_INIT_STATE          (0x04)

#define APPLICATION_VERSION      "1.1.1"
#define APPLICATION_NAME         "Embedded MQTT Client"

#define SLNET_IF_WIFI_PRIO       (5)

/* Operate Lib in MQTT 3.1 mode.                                             */
#define MQTT_3_1_1               false
#define MQTT_3_1                 true

#define WILL_TOPIC               "Client"
#define WILL_MSG                 "Client Stopped"
#define WILL_QOS                 MQTT_QOS_2
#define WILL_RETAIN              false

/* Defining Broker IP address and port Number                                */
//#define SERVER_ADDRESS           "messagesight.demos.ibm.com"
#define SERVER_ADDRESS           "192.168.2.1"
#define SERVER_IP_ADDRESS        "192.168.2.1"
#define PORT_NUMBER              1883
#define SECURED_PORT_NUMBER      8883
#define LOOPBACK_PORT            1882

/* Clean session flag                                                        */
#define CLEAN_SESSION            true

/* Retain Flag. Used in publish message.                                     */
#define RETAIN_ENABLE            1

/* Defining Number of subscription topics                                    */
#define SUBSCRIPTION_TOPIC_COUNT 1

/* Defining Subscription Topic Values                                        */
#define SUBSCRIPTION_TOPIC0      "/team20/tri_state"

/* Defining Publish Topic Values                                             */
#define PUBLISH_TOPIC_BUFFER_SIZE (40)
#define PUBLISH_TOPIC_COUNT (3)
#define PUBLISH_TOPIC0           "/team20/zumo_state"
#define PUBLISH_TOPIC1           "/team20/zumo_leftmotor"
#define PUBLISH_TOPIC2           "/team20/zumo_rightmotor"

/* Spawn task priority and Task and Thread Stack Size                        */
#define TASKSTACKSIZE            2048
#define RXTASKSIZE               4096
#define MQTTTHREADSIZE           2048
#define SPAWN_TASK_PRIORITY      9

/* secured client requires time configuration, in order to verify server     */
/* certificate validity (date).                                              */

/* Day of month (DD format) range 1-31                                       */
#define DAY                      1
/* Month (MM format) in the range of 1-12                                    */
#define MONTH                    5
/* Year (YYYY format)                                                        */
#define YEAR                     2017
/* Hours in the range of 0-23                                                */
#define HOUR                     12
/* Minutes in the range of 0-59                                              */
#define MINUTES                  33
/* Seconds in the range of 0-59                                              */
#define SEC                      21

/* Number of files used for secure connection                                */
#define CLIENT_NUM_SECURE_FILES  1

/* Expiration value for the timer that is being used to toggle the Led.      */
#define TIMER_EXPIRATION_VALUE   100 * 1000000

#define CLIENTID_SIZE (13)
#define PUBLISH_PAYLOAD_SIZE (400)
//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//*****************************************************************************
void TimerPeriodicIntHandler(sigval val);
void LedTimerConfigNStart();
void LedTimerDeinitStop();
void DisplayBanner(char * AppName);
void *MqttClient(void *pvParameters);
void Mqtt_ClientStop(uint8_t disconnect);
void Mqtt_ServerStop();
void Mqtt_Stop();
void Mqtt_start();
int32_t Mqtt_IF_Connect();
int32_t MqttServer_start();
int32_t MqttClient_start();
void *commThread(void * args);

#endif /* MQTT_CLIENT_H_ */

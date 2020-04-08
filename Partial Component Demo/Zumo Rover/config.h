/*
 * config.h
 *
 *  Created on: Apr 5, 2020
 *      Author: Holden Ramsey
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "debug.h"
#include "queue.h"

void * configThread(void *arg0);
void sendMsgToConfigQ(int msg);
void receiveFromConfigQ(int *received);
void createConfigQueue();

#endif /* CONFIG_H_ */

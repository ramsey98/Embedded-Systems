/*
 * sensor_queue.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef SENSOR_QUEUE_H_
#define SENSOR_QUEUE_H_


int sendTimeMsgToQ1(unsigned int timeVal);
int sendSensorMsgToQ1(int mmDist);


#endif /* SENSOR_QUEUE_H_ */

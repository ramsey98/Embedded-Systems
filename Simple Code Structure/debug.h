/*
 * debug.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"

#define DBG_ERROR (127)

void dbgUARTVal(unsigned char outVal);
void dbgOutputLoc(unsigned int outVal);
void halt();


#endif /* DEBUG_H_ */

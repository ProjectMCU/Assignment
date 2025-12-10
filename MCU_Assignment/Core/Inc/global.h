/*
 * global.h
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "software_timer.h"
#include "button.h"
#include "scheduler.h"
#include "LCD.h"
#include "FSM.h"

#define INIT		1
#define AUTO_RED	2
#define AUTO_YELLOW	3
#define AUTO_GREEN	4


#define MANUAL		5

extern int RED_TIME;
extern int GREEN_TIME;
extern int YELLOW_TIME;

extern int MODE;
extern int status;

extern int duration;

extern int displayMode;
extern int displayValue;

#endif /* INC_GLOBAL_H_ */

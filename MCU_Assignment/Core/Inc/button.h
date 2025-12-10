/*
 * button.h
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"
#include "software_timer.h"
#define NORMAL_STATE SET
#define PRESSED_STATE RESET
#define MAX_BUTTON	2


extern int button_flag[2];

extern int button_pressed[2];

extern int button_long_pressed[2];

int isButtonPressed(int index);
int isButtonLongPressed(int index);
void getKeyInput();

#endif /* INC_BUTTON_H_ */

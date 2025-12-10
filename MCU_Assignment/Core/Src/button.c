/*
 * button.c
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#include "button.h"

int KeyReg0[MAX_BUTTON] = {NORMAL_STATE, NORMAL_STATE};
int KeyReg1[MAX_BUTTON] = {NORMAL_STATE, NORMAL_STATE};
int KeyReg2[MAX_BUTTON]	= {NORMAL_STATE, NORMAL_STATE};
int KeyReg3[MAX_BUTTON]	= {NORMAL_STATE, NORMAL_STATE};

int TimeOutForKeyPress = 100;
int button_flag[MAX_BUTTON] = {0, 0};
int button_long_pressed[MAX_BUTTON] = {0, 0};


int isButtonPressed(int index){
	if (button_long_pressed[index] == 1) return 0;
	if (button_flag[index] == 1){
		button_flag[index] = 0;
		return 1;
	}
	return 0;
}

int isButtonLongPressed(int index){
	if(button_long_pressed[index] == 1){
		button_long_pressed[index] = 0;
		return 1;
	}
	return 0;
}


void getKeyInput(void) {
    // -------- BUTTON 1 (short press) --------
    KeyReg0[0] = KeyReg1[0];
    KeyReg1[0] = KeyReg2[0];
    KeyReg2[0] = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);

    if ((KeyReg0[0] == KeyReg1[0]) && (KeyReg1[0] == KeyReg2[0])) {
        if (KeyReg3[0] != KeyReg2[0]) {
            KeyReg3[0] = KeyReg2[0];
            if (KeyReg2[0] == PRESSED_STATE) button_flag[0] = 1;
        }
    }

    // -------- BUTTON 2 (short press) --------
    KeyReg0[1] = KeyReg1[1];
    KeyReg1[1] = KeyReg2[1];
    KeyReg2[1] = HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin);

    if ((KeyReg0[1] == KeyReg1[1]) && (KeyReg1[1] == KeyReg2[1])) {
        if (KeyReg3[1] != KeyReg2[1]) {
            KeyReg3[1] = KeyReg2[1];
            if (KeyReg3[1] == PRESSED_STATE) button_flag[1] = 1;
        }
    }

    // -------- BUTTON 3 (short + long press) --------
//    KeyReg0[2] = KeyReg1[2];
//    KeyReg1[2] = KeyReg2[2];
//    KeyReg2[2] = HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin);
//
//    if ((KeyReg0[2] == KeyReg1[2]) && (KeyReg1[2] == KeyReg2[2])) {
//        if (KeyReg3[2] != KeyReg2[2]) {
//            KeyReg3[2] = KeyReg2[2];
//
//            if (KeyReg3[2] == PRESSED_STATE) {
//                button_long_pressed[2] = 0;
//                TimeOutForKeyPress = 100;
//            } else {
//                if (TimeOutForKeyPress > 0) {
//                    button_flag[2] = 1;
//                }
//                TimeOutForKeyPress = 100;
//            }
//        } else {
//            if (KeyReg2[2] == PRESSED_STATE) {
//                if (TimeOutForKeyPress > 0) TimeOutForKeyPress--;
//
//                if (TimeOutForKeyPress == 0) {
//                    button_long_pressed[2] = 1;
//                    button_flag[2] = 0;
//                }
//            }
//        }
//    }
}





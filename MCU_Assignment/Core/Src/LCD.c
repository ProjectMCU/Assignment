/*
 * LCD.c
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#include "LCD.h"
#include <stdio.h>
#include <string.h>

void send8bitLCD(char d){
	int b0,b1,b2,b3,b4,b5,b6,b7;
	if ((d & 1) == 0){
		b0 = 0;
	} else {
		b0 = 1;
	}

	if ((d & 2) == 0){
		b1 = 0;
	} else {
		b1 = 1;
	}

	if ((d & 4) == 0){
		b2 = 0;
	} else {
		b2 = 1;
	}

	if ((d & 8) == 0){
		b3 = 0;
	} else {
		b3 = 1;
	}

	if ((d & 16) == 0){
		b4 = 0;
	} else {
		b4 = 1;
	}

	if ((d & 32) == 0){
		b5 = 0;
	} else {
		b5 = 1;
	}

	if ((d & 64) == 0){
		b6 = 0;
	} else {
		b6 = 1;
	}

	if ((d & 128) == 0){
		b7 = 0;
	} else {
		b7 = 1;
	}

	HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, b0);
	HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, b1);
	HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, b2);
	HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, b3);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, b4);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, b5);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, b6);
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, b7);
}

void sendcmd2LCD(char cmd){
	HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
	send8bitLCD(cmd);
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
}

void sendchar2LCD(char Char){
	HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
	send8bitLCD(Char);
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
}

void sendString2LCD(char *str){
	for (int i=0; str[i] != '\0'; i++) {
		sendchar2LCD(str[i]);
	}
}

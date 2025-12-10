/*
 * FSM.c
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */
#include "FSM.h"

void Auto(){
	if (MODE == 1){
		switch(status){
		case INIT:
			setTimer(0, RED_TIME * 100);
//			setTimer(1, 100); for display in LCD
			status = AUTO_RED;
			break;
		case AUTO_RED:
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			if (timer_flag[0] == 1){
				setTimer(0, GREEN_TIME * 100);
				status = AUTO_GREEN;
			}
			break;
		case AUTO_GREEN:
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			if (timer_flag[0] == 1){
				setTimer(0, YELLOW_TIME * 100);
				status = AUTO_YELLOW;
			}
			break;
		case AUTO_YELLOW:
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
			if (timer_flag[0] == 1){
				setTimer(0, RED_TIME * 100);
				status = AUTO_RED;
			}
			break;
		default:
			break;
		}
	}
}

void led_Blink(){
	if (timer_flag[3] == 1) {
	        timer_flag[3] = 0;

	        switch (MODE) {
	            case 2: // RED blink
	                HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	                break;

	            case 4: // GREEN blink
	                HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	                break;

	            case 3: // YELLOW blink
	                HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
	                break;

	            default: break;
	        }

	        setTimer(3, 25);
	    }
}


void Config(){
	if (isButtonPressed(1)){
		if (MODE == 1){
			MODE = 2;
			displayMode = 2;
			displayValue = RED_TIME;
		}
		else {
			switch(MODE){
			case 2:
				MODE = 3;
				displayMode = 3;
				displayValue = YELLOW_TIME;
				break;
			case 3:
				MODE = 4;
				displayMode = 4;
				displayValue = GREEN_TIME;
				break;
			case 4:
				MODE = 2;
				displayMode = 2;
				displayValue = RED_TIME;
				break;
			}
		}

		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

		setTimer(3, 25);
	}


	if (MODE >= 2 && MODE <= 4){
		led_Blink();
	}

    if(isButtonPressed(2)){
        displayValue++;
        if(displayValue > 99) displayValue = 99;
        switch(MODE){
            case 2: RED_TIME    = displayValue; break;
            case 3: GREEN_TIME  = displayValue; break;
            case 4: YELLOW_TIME = displayValue; break;
        }
    }
    if(isButtonLongPressed(2)){
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

        MODE = 1;
        status = INIT;
    }
}

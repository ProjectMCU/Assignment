/*
 * software_timer.c
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#include "software_timer.h"

#define MAX_TIMER 10

int timer_counter[MAX_TIMER] = {0} ;
int timer_flag[MAX_TIMER] = {0};

void setTimer(int index, int duration){
	if (index < 0 || index >= MAX_TIMER) return;
	timer_counter[index] = duration;
	timer_flag[index] = 0;
}

void timerRun() {
    for (int i = 0; i < MAX_TIMER; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) {
                timer_flag[i] = 1;
            }
        }
    }
}

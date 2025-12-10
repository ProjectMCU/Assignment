/*
 * software_timer.h
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define MAX_TIMER 10

extern int timer_flag[MAX_TIMER];
extern int timer_counter[MAX_TIMER] ;


void setTimer(int index, int duration);
void timerRun();

#endif /* INC_SOFTWARE_TIMER_H_ */

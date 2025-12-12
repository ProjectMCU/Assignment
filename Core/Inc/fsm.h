#ifndef FSM_H
#define FSM_H

#include "main.h" 
#include "button.h"

// Định nghĩa các MODE
#define MODE_1 1 // AUTO MODE
#define MODE_2 2 // CONFIG: RED
#define MODE_3 3 // CONFIG: YELLOW
#define MODE_4 4 // CONFIG: GREEN

enum {
    STATE_R1_G2, // 0: Đỏ 1, Xanh 2
    STATE_R1_Y2, // 1: Đỏ 1, Vàng 2
    STATE_G1_R2, // 2: Xanh 1, Đỏ 2
    STATE_Y1_R2, // 3: Vàng 1, Đỏ 2
    NUM_AUTO_STATES
};

typedef struct {
    int r1, y1, g1;
    int r2, y2, g2;
    int duration;
    int next_state;
} TrafficState_t;

void enterState(int new_state,int r1, int y1, int g1,int r2, int y2, int g2);
void check_button(void);
void Task_FSM(void);
void Task_BlinkLED(void);
void setTrafficLED(int r1, int y1, int g1, int r2, int y2, int g2);
void lcd_show_auto_mode(int t1, int t2);
void lcd_show_config(int mode, int inc_value);

#endif 

#include "fsm.h"
#include "i2c-lcd.h"
#include "main.h"
#include <stdio.h>
#include <string.h>


// Dữ liệu FSM (Bảng Trạng thái) 
const TrafficState_t FSM_Table[NUM_AUTO_STATES] = {
    // 0: STATE_R1_G2 (Đỏ 1, Xanh 2) -> L2 đếm Green Time
    {1, 0, 0, 0, 0, 1, 0, STATE_R1_Y2},
    // 1: STATE_R1_Y2 (Đỏ 1, Vàng 2) -> L2 đếm Yellow Time
    {1, 0, 0, 0, 1, 0, 0, STATE_G1_R2},
    // 2: STATE_G1_R2 (Xanh 1, Đỏ 2) -> L1 đếm Green Time
    {0, 0, 1, 1, 0, 0, 0, STATE_Y1_R2},
    // 3: STATE_Y1_R2 (Vàng 1, Đỏ 2) -> L1 đếm Yellow Time
    {0, 1, 0, 1, 0, 0, 0, STATE_R1_G2}
};

int status = STATE_R1_G2;
int mode   = MODE_1;

int current_time_1;
int current_time_2;

static int red_time    = 5;
static int yellow_time = 2;
static int green_time  = 3;

static int config_value = 0;
static int blink_flag   = 0;

static int cnt = 0;


static char lcd_line0[17] = "";
static char lcd_line1[17] = "";

// LCD Helpers 

static void lcd_update_line(int row, const char* text) {
    char* buf = (row == 0) ? lcd_line0 : lcd_line1;
    if (strcmp(buf, text) == 0) return;

    strcpy(buf, text);
    lcd_goto_XY(row + 1, 0);
    lcd_send_string((char*)text);
}

void lcd_show_auto_mode(int t1, int t2) {
    char line0[17];
    char line1[17];

    sprintf(line0, " AUTO MODE     ");
    sprintf(line1, "R1:%02d  R2:%02d ", t1, t2);

    lcd_update_line(0, line0);
    lcd_update_line(1, line1);
}

void lcd_show_config(int current_mode, int inc_value) {
    char line0[17];
    char line1[17];

    int set_value =
        (current_mode == MODE_2) ? red_time :
        (current_mode == MODE_3) ? yellow_time :
                                   green_time;

    if (current_mode == MODE_2)       sprintf(line0, "CONFIG: RED    ");
    else if (current_mode == MODE_3)  sprintf(line0, "CONFIG: YELLOW ");
    else if (current_mode == MODE_4)  sprintf(line0, "CONFIG: GREEN  ");
    else                              sprintf(line0, "CONFIG:        ");

    sprintf(line1, "INC:%02d SET:%02d ", inc_value, set_value);

    lcd_update_line(0, line0);
    lcd_update_line(1, line1);
}

// LED Control (Logic 2-bit) 

static void setModuleLED(int color,
                         GPIO_TypeDef* PA, uint16_t A,
                         GPIO_TypeDef* PB, uint16_t B)
{
    HAL_GPIO_WritePin(PA, A, (color & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PB, B, (color & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void setTrafficLED(int r1, int y1, int g1, int r2, int y2, int g2)
{
    uint8_t code1 = (r1 ? 3 : y1 ? 1 : g1 ? 2 : 0);
    uint8_t code2 = (r2 ? 3 : y2 ? 1 : g2 ? 2 : 0);

    setModuleLED(code1, L1A_GPIO_Port, L1A_Pin, L1B_GPIO_Port, L1B_Pin);
    setModuleLED(code2, L2A_GPIO_Port, L2A_Pin, L2B_GPIO_Port, L2B_Pin);
}

// FSM Handler 

void enterState(int new_state,
                int r1, int y1, int g1,
                int r2, int y2, int g2)
{
    status = new_state;
    setTrafficLED(r1, y1, g1, r2, y2, g2);
}

// BUTTON HANDLER 
void check_button(void)
{
    int button_event = isButtonPressed();
    if (button_event == 0) return;

    if (button_event == 1) // MODE
    {
        mode = (mode == MODE_4) ? MODE_1 : mode + 1;

        cnt = 0;
        config_value = 0;

        lcd_clear_display();
        lcd_line0[0] = 0;
        lcd_line1[0] = 0;

        if (mode == MODE_1)
        {
            status = STATE_R1_G2;
            current_time_1 = red_time;
            current_time_2 = green_time;
        } else {
            setTrafficLED(0,0,0, 0,0,0);
        }
        return;
    }

    if (mode == MODE_1) return;

    if (button_event == 2) // INCREASE
    {
        cnt = (cnt + 1) % 100;
        config_value = cnt;
    }
    else if (button_event == 3) // SET
    {
        switch (mode)
        {
            case MODE_2: red_time = cnt; break;
            case MODE_3: yellow_time = cnt; break;
            case MODE_4:
                if (red_time == yellow_time + cnt) {
                    green_time = cnt;
                } else {
                    red_time = 5;
                    yellow_time = 2;
                    green_time = 3;
                }
                break;
        }
        cnt = 0;
        config_value = 0;
    }

    lcd_show_config(mode, config_value);
}

// FSM CỦA AUTO MODE (MODE_1) 

static void run_auto_fsm(void)
{
    lcd_show_auto_mode(current_time_1, current_time_2);

    // GIẢM THỜI GIAN ĐẾM NGƯỢC (CẢ HAI CÙNG GIẢM) 
    if (current_time_1 > 0) current_time_1--;
    if (current_time_2 > 0) current_time_2--;


    // CHUYỂN TRẠNG THÁI 
    switch (status)
    {
        case STATE_R1_G2: 
            if (current_time_2 <= 0) {
                enterState(STATE_R1_Y2, 1,0,0, 0,1,0); 
                current_time_2 = yellow_time;
            } else {
                enterState(STATE_R1_G2, 1,0,0, 0,0,1);
            }
            break;

        case STATE_R1_Y2: 
            if (current_time_2 <= 0) {
                enterState(STATE_G1_R2, 0,0,1, 1,0,0); 
                current_time_1 = green_time;           
                current_time_2 = red_time;           
            } else {
                enterState(STATE_R1_Y2, 1,0,0, 0,1,0);
            }
            break;

        case STATE_G1_R2: 
            if (current_time_1 <= 0) {
                enterState(STATE_Y1_R2, 0,1,0, 1,0,0); 
                current_time_1 = yellow_time;
            } else {
                enterState(STATE_G1_R2, 0,0,1, 1,0,0);
            }
            break;

        case STATE_Y1_R2: 
            if (current_time_1 <= 0) {
                enterState(STATE_R1_G2, 1,0,0, 0,0,1); 
                current_time_1 = red_time;           
                current_time_2 = green_time;         
            } else {
                enterState(STATE_Y1_R2, 0,1,0, 1,0,0);
            }
            break;
    }
}

// CONFIG MODE HANDLER (BLINKS) 

static void run_config_blink(void)
{
    blink_flag = !blink_flag;

    int r = (mode == MODE_2);
    int y = (mode == MODE_3);
    int g = (mode == MODE_4);

    if (blink_flag) {
        setTrafficLED(r, y, g, r, y, g); 
    } else {
        setTrafficLED(0, 0, 0, 0, 0, 0);
    }

    lcd_show_config(mode, config_value);
}

// TASKS 

void Task_FSM(void)
{
    check_button();
    if (mode == MODE_1)
        run_auto_fsm();
}

void Task_BlinkLED(void)
{
    if (mode != MODE_1)
        run_config_blink();
}

/*
 * button.c
 * Chức năng: Đọc trạng thái các nút nhấn và thực hiện chống dội (debounce) bằng phần mềm.
 */

#include "button.h"
#include "fsm.h"

// Định nghĩa
#define NO_BUTTON_PRESSED    0
#define BUTTON_MODE_PRESSED  1
#define BUTTON_INC_PRESSED   2
#define BUTTON_SET_PRESSED   3

#define NUM_BUTTONS          3

static int previous_states[NUM_BUTTONS] = {
    GPIO_PIN_SET, // MODE
    GPIO_PIN_SET, // INC
    GPIO_PIN_SET  // SET
};


int isButtonPressed(void)
{
    GPIO_TypeDef* ports[] = {BUTTON1_GPIO_Port, BUTTON2_GPIO_Port, BUTTON3_GPIO_Port};
    uint16_t pins[] = {BUTTON1_Pin, BUTTON2_Pin, BUTTON3_Pin};
    int button_id_map[] = {BUTTON_MODE_PRESSED, BUTTON_INC_PRESSED, BUTTON_SET_PRESSED}; 

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        int current_state = HAL_GPIO_ReadPin(ports[i], pins[i]);
        if (current_state == GPIO_PIN_RESET && previous_states[i] == GPIO_PIN_SET)
        {
            previous_states[i] = GPIO_PIN_RESET;
            return button_id_map[i];
        }
        if (current_state == GPIO_PIN_SET)
        {
            previous_states[i] = GPIO_PIN_SET;
        }
    }

    return NO_BUTTON_PRESSED;
}


void Task_Button_Read(void)
{
    check_button();
}

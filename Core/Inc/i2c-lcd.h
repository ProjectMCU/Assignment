#include "stm32f1xx_hal.h"

void lcd_init (void);  

void lcd_send_cmd (char cmd);  

void lcd_send_data (char data); 

void lcd_send_string (char *str); 

void lcd_clear_display (void);

void lcd_goto_XY (int row, int col); 


void lcd_interface_auto();
void lcd_counter(int road, int n);

void lcd_interface_settings();
void lcd_increase_and_set(int select, int n);

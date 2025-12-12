#include "i2c-lcd.h"
#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;  

#define SLAVE_ADDRESS_LCD (0x21 << 1)

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  
	data_t[1] = data_u|0x08; 
	data_t[2] = data_l|0x0C; 
	data_t[3] = data_l|0x08; 
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  
	data_t[1] = data_u|0x09; 
	data_t[2] = data_l|0x0D;
	data_t[3] = data_l|0x09; 
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void) {
	lcd_send_cmd (0x33);
	lcd_send_cmd (0x32);
	HAL_Delay(2);
	lcd_send_cmd (0x28); 
	HAL_Delay(2);
	lcd_send_cmd (0x01); 
	HAL_Delay(2);
	lcd_send_cmd (0x06);
	HAL_Delay(2);
	lcd_send_cmd (0x0c); 
	HAL_Delay(2);
	lcd_send_cmd (0x02);
	HAL_Delay(2);
	lcd_send_cmd (0x80);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); 
	HAL_Delay(2);
}

void lcd_goto_XY (int row, int col)
{
	uint8_t pos_Addr;
	if(row == 1)
	{
		pos_Addr = 0x80 + row - 1 + col;
	}
	else
	{
		pos_Addr = 0x80 | (0x40 + col);
	}
	lcd_send_cmd(pos_Addr);

	HAL_Delay(2);
}

void lcd_interface_auto() {
	lcd_goto_XY(1, 0);
	lcd_send_string("AUTO MODE");
	lcd_goto_XY(2, 0);
	lcd_send_string("R1:");
	lcd_goto_XY(2, 11);
	lcd_send_string("R2:");
}

void lcd_counter(int road, int n) {
	if (road == 1) {
		lcd_goto_XY(2, 3);
		lcd_send_data('0' + n/10);
		lcd_send_data('0' + n%10);
	}
	else if (road == 2) {
		lcd_goto_XY(2, 14);
		lcd_send_data('0' + n/10);
		lcd_send_data('0' + n%10);
	}
}

void lcd_interface_settings() {
	lcd_goto_XY(2, 0);
	lcd_send_string("IN:");
	lcd_goto_XY(2, 10);
	lcd_send_string("SET:");
}

void lcd_increase_and_set(int select, int n) {
	switch (select) {
		case 1:
			lcd_goto_XY(2, 3);
			lcd_send_data('0' + n/10);
			lcd_send_data('0' + n%10);
			break;
		case 2:
			lcd_goto_XY(2, 14);
			lcd_send_data('0' + n/10);
			lcd_send_data('0' + n%10);
			break;
		default:
			break;
	}
}

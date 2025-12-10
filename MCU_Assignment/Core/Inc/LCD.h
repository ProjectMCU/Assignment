/*
 * LCD.h
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>
#include "global.h"
void send8bitLCD(char d);
void sendcmd2LCD(char cmd);
void sendchar2LCD(char Char);
void sendString2LCD(char *str);
#endif /* LCD_H */

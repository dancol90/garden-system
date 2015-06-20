/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __display_h__
#define __display_h__

#include <Arduino.h>

#include <Wire.h>
#include <LightLCD.h>
#include <LightSSD1306.h>

void init_display();
void update_display();

void start_backlight_timer();

// Common methods, non driver-dependant
#include "digit_font.h"

void draw_big_digit(uint8_t xPos, uint8_t yPos, uint8_t digit);

extern LightSSD1306 lcd;

#endif
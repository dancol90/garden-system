/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __display_h__
#define __display_h__

#include <Arduino.h>
#include <LightLCD.h>
#include "../config/config.h"

#if defined USE_SSD1306
	#include <LightSSD1306.h>
#elif defined USE_PCD8544
	#include <LightPCD8544.h>
#endif

void init_display();
void update_display();

void start_backlight_timer();

// Common methods, non driver-dependant
#include "digit_font.h"

void draw_big_digit(uint8_t xPos, uint8_t yPos, uint8_t digit);

extern LightLCD& lcd;

#endif
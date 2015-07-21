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
void display_init();
void display_update();

void init_display();
void update_display();
void display_start_backlight_timer();

// Common methods, non driver-dependant
#include "digit_font.h"

void display_draw_big_digit(uint8_t xPos, uint8_t yPos, uint8_t digit);
void display_show_message(const __FlashStringHelper* msg);

void display_draw_selector(int x, int y);

extern LightLCD& lcd;

#endif
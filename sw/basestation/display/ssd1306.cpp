/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include <Wire.h>
#include "display.h"

#ifdef USE_SSD1306

LightLCD& lcd = *new LightSSD1306();

void init_display() { 
	Wire.begin();
    lcd.begin();
}

void update_display() {}
void start_backlight_timer() {}

#endif
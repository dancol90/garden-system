/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "display.h"

LightSSD1306 lcd = LightSSD1306();

void init_display() { 
    lcd.begin();
}

void update_display() {}
void start_backlight_timer() {}

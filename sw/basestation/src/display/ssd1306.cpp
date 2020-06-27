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

void display_init() { 
    Wire.begin(SSD1306_SDA_PIN, SSD1306_SCL_PIN);
    lcd.begin();
}

void display_update() {}
void display_start_backlight_timer() {}

#endif
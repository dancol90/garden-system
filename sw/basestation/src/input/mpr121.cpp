/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "input.h"

#ifdef USE_INPUT_MPR121

#include <Wire.h>
#include <LightMPR121.h>

LightMPR121 touch;
int last_status;
long repeat_timer;
bool changed;

void input_init() {
	Wire.begin();
    touch.begin();
}

bool input_update() {
    int new_status = touch.update();

    changed = new_status != last_status;

    last_status = new_status;

    if (changed || millis() - repeat_timer > 150) {
    	repeat_timer = millis();
		if(!changed) changed = true;
    }
}

bool input_is_pressed(uint8_t i) { return touch.touched(i) && changed; }
bool input_released  (uint8_t i) { return true; }

#endif
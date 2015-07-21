/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include <Wire.h>
#include "input.h"

LightMPR121 touch;
int last_status;
long repeat_timer;
bool changed;

void init_buttons() {
	Wire.begin();
    touch.begin();
}

bool update_buttons() {
    int new_status = touch.update();

    changed = new_status != last_status;

    last_status = new_status;

    if (changed || millis() - repeat_timer > 150) {
    	repeat_timer = millis();
		if(!changed) changed = true;
    }
}

bool is_pressed(uint8_t i) { return touch.touched(i) && changed; }
bool   released(uint8_t i) { return true; }

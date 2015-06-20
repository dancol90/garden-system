/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "input.h"

LightMPR121 touch;

void init_buttons() {
    touch.begin();
}

bool update_buttons() {
    touch.update();
}

bool is_pressed(uint8_t i) { return touch.touched(i); }
bool   released(uint8_t i) { return true; }

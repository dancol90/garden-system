/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __input_h
#define __input_h

#include <Arduino.h>
#include <LightMPR121.h>

void init_buttons();
bool update_buttons();

bool is_pressed(uint8_t i);
bool   released(uint8_t i);

struct ButtonState {
    uint8_t changed :1;
    uint8_t state   :1;
    
    uint8_t dirty   :1;
    uint8_t read    :1;
    long timer;
};

extern LightMPR121 touch;

#endif
/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __rtc_h__
#define __rtc_h__

#include <Arduino.h>
#include <Wire.h>

#include "../config/config.h"
#include "../settings.h"

union Time {
    uint16_t minutes;
    struct {
        uint8_t m;
        uint8_t h;
    } s;
};

struct Interval {
    Time start;
    Time end;

    bool enabled : 1;
};

void init_rtc();

void read_time();
void write_time(Time t);

extern Time now;

#endif
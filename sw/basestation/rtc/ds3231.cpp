/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "rtc.h"
#include <Wire.h>

#ifdef USE_DS3231_RTC

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

DateTime now;
uint8_t last_minute = 0;

void update_rtc() {
    if (state.rtc_stop) return;

    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(0x68, 3);

    now.time.second = bcd2bin(Wire.read());
    now.time.minute = bcd2bin(Wire.read());
    now.time.hour   = bcd2bin(Wire.read());

    state.new_minute = (last_minute != now.s.minute);
    last_minute = now.s.minute;
}

void write_time(DateTime t) {
    Wire.beginTransmission(0x68);
    // First register, seconds
    Wire.write(0x00);

    // Reset seconds
    Wire.write(0);
    // Write mins and hours
    Wire.write(bin2bcd(t.time.minute));
    Wire.write(bin2bcd(t.time.hour));

    Wire.endTransmission();
}

void init_rtc() {
    Wire.beginTransmission(0x68);

    // Enable crystal.
    // TODO: Is it really needed?
    Wire.write(0x0e);
    Wire.write(0);

    Wire.endTransmission();
}

#endif
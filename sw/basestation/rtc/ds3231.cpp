/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "rtc.h"
#include <Wire.h>

#ifdef USE_DS3231_RTC

static uint8_t _bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t _bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

DateTime rtc_now;
uint8_t _last_minute = 0;

void rtc_update() {
    if (state.rtc_stop) return;

    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(0x68, 3);

    rtc_now.time.second = _bcd2bin(Wire.read());
    rtc_now.time.minute = _bcd2bin(Wire.read());
    rtc_now.time.hour   = _bcd2bin(Wire.read());

    state.new_minute = (_last_minute != rtc_now.s.minute);
    _last_minute = rtc_now.s.minute;
}

void rtc_write_time(DateTime t) {
    Wire.beginTransmission(0x68);
    // First register, seconds
    Wire.write(0x00);

    // Reset seconds
    Wire.write(0);
    // Write mins and hours
    Wire.write(_bin2bcd(t.time.minute));
    Wire.write(_bin2bcd(t.time.hour));

    Wire.endTransmission();
}

void rtc_init() {
    Wire.beginTransmission(0x68);

    // Enable crystal.
    // TODO: Is it really needed?
    Wire.write(0x0e);
    Wire.write(0);

    Wire.endTransmission();
}

#endif
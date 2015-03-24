/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }

void read_time() {
    Wire.beginTransmission(0x68);
    // Skip seconds register and start from minutes
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(0x68, 2);

    now.s.m = bcd2bin(Wire.read());
    now.s.h = bcd2bin(Wire.read());
}

void init_rtc() {
    Wire.beginTransmission(0x68);

    // Enable crystal.
    // TODO: Is it really needed?
    Wire.write(0x0e);
    Wire.write(0);

    Wire.endTransmission();
}
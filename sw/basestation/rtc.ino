/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

uint8_t last_minute = 0;

void read_time() {
    if (state.rtc_stop) return;

    Wire.beginTransmission(0x68);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(0x68, 2);

    now.s.m = bcd2bin(Wire.read());
    now.s.h = bcd2bin(Wire.read());

    state.new_minute = (last_minute != now.s.m);
    last_minute = now.s.m;
}

void write_time(Time t) {
    Wire.beginTransmission(0x68);
    // First register, seconds
    Wire.write(0x00);

    // Reset seconds
    Wire.write(0);
    // Write mins and hours
    Wire.write(bin2bcd(t.s.m));
    Wire.write(bin2bcd(t.s.h));

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
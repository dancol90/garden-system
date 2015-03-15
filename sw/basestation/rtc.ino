
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }

void read_time() {
    Wire.beginTransmission(0x68);
    Wire.write(1);   
    Wire.endTransmission();

    Wire.requestFrom(0x68, 2);
    now.s.m = bcd2bin(Wire.read());
    now.s.h = bcd2bin(Wire.read());
}
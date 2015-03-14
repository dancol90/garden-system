
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }

void read_time() {
    Wire.beginTransmission(0x68);
    Wire.write(0);   
    Wire.endTransmission();

    Wire.requestFrom(0x68, 3);
    //uint8_t ss = bcd2bin(Wire.read() & 0x7F);
    Wire.read();
    now.s.h = bcd2bin(Wire.read());
    now.s.m = bcd2bin(Wire.read());
    /*
    Wire.read();
    uint8_t d = bcd2bin(Wire.read());
    uint8_t m = bcd2bin(Wire.read());
    uint16_t y = bcd2bin(Wire.read()) + 2000;
    */
}
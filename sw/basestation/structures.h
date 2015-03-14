
union Time {
    uint16_t minutes;
    struct {
        uint8_t m;
        uint8_t h;
    } s;
};

struct ButtonState {
    uint8_t changed :1;
    uint8_t state   :1;
    
    uint8_t read    :1;
    uint8_t timer;
};
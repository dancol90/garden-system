
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
    long timer;
};


struct ApplicationState {
	uint8_t contrast;
    uint8_t backlight;

    uint8_t : 0;

    uint8_t receiver_no : 2;
    uint8_t menu_active : 1;
};

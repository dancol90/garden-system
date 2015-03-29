/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

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

    uint8_t selected_recv : 3;
    uint8_t menu_active : 1;
};


struct ReceiverState {
    bool active;
    Interval jobs[10];
};
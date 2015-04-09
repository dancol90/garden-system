/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef structures_h
#define structures_h

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

struct ButtonState {
    uint8_t changed :1;
    uint8_t state   :1;
    
    uint8_t read    :1;
    long timer;
};


struct ApplicationState {
    uint8_t selected_recv : 3; // The index of the selected receiver
    bool menu_active : 1;      // Indicates if the menu is being shown or not
    bool rtc_stop : 1;         // Whether prevent reading from rtc or not

    bool new_minute : 1;
    bool force_schedule_update : 1;
    bool lcd_settings_dirty : 1;
};

struct Settings {
    uint8_t contrast;
    uint8_t backlight;
};


struct ReceiverState {
    bool active : 1;

    byte :0;

    int8_t current_job : 4;
    int8_t    next_job : 4;
};

struct Packet {
    uint8_t command;
    uint8_t id;
    uint8_t state;
};

#endif

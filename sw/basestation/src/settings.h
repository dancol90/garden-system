/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __settings_h__
#define __settings_h__

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

extern ApplicationState state;
extern Settings settings;

#endif
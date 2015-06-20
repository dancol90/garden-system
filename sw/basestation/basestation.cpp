/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "basestation.h"

// ############################################################################################
ApplicationState state;
Settings settings;

// ############################################################################################
void setup() {
    Serial.begin(115200);

    init_eeprom();

    load_settings();

    // Clear data
    memset(&state,    0, sizeof(ApplicationState));
    memset(&receivers, 0, sizeof(ReceiverState) * RECEIVER_COUNT);

    init_rtc();

    Wire.begin();

    //init_rf();

    init_buttons();
    init_display();

    //int ram = free_ram();

    init_menu();

    //Serial.print(F("before: ")); Serial.println(ram);
    //Serial.print(F("after:  ")); Serial.println(free_ram());
    //Serial.print(F("diff:   ")); Serial.println(ram - free_ram());

    state.force_schedule_update = true;
}

void loop() {

    // Update time
    read_time();

    if (state.new_minute || state.force_schedule_update) {
        update_schedule();
    }

    update_rf();

    // Update button states
    if (update_buttons()) {
        start_backlight_timer();
    }

    if (state.menu_active) {
        update_menu();
    } else {
        // Update homescreen state
        update_homescreen();
    }

    update_display();

    delay(50);
}

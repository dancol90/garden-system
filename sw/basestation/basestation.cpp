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

    init_rf();

    init_buttons();
    init_display();

    init_menu();

    state.force_schedule_update = true;

    Serial.println(sizeof(Interval));
}

void loop() {

    // Update time
    update_rtc();

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

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

    storage_init();

    storage_load_settings();

    // Clear data
    memset(&state,    0, sizeof(ApplicationState));
    memset(&receivers, 0, sizeof(ReceiverState) * RECEIVER_COUNT);

    rtc_init();

    rf_init();

    input_init();
    display_init();

    menu_init();

#ifdef USE_WIFI
    wifi_init();
#endif

    state.force_schedule_update = true;
}

long start_t;

void loop() {

    // Update time
    rtc_update();

    if (state.new_minute || state.force_schedule_update) {
        schedule_update();
    }

    rf_update();

    // Update button states
    if (input_update()) {
        display_start_backlight_timer();
    }

    if (state.menu_active) {
        menu_update();
    } else {
        // Update homescreen state
        home_update();
    }

    display_update();

#ifdef USE_WIFI
    wifi_update();
#endif

    delay(50);
}

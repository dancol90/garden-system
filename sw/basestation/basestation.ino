/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

#include <LightPCD8544.h>
#include "nRF24L01.h"
#include "RF24.h"

#include <MenuLib.h>
#include <NokiaLcdDrawer.h>

#include "structures.h"
#include "digit_font.h"

#if defined(AVR)
	#include "config_arduino.h"
#elif defined(ESP8266)
	#include "config_esp8266.h"
#else
	#error "No compatible platform defined. Check your settings."
#endif

#define selected_receiver receivers[state.selected_recv]

// ############################################################################################

Time now;
ApplicationState state;
Settings settings;

ReceiverState receivers[RECEIVER_COUNT];

LightPCD8544 lcd = LightPCD8544(LCD_DC, LCD_CS);
MenuController menu;

#include "menu_custom.h"

// ############################################################################################

void setup() {
    Serial.begin(9600);

    init_eeprom();

    load_settings();

    // Clear data
    memset(&state,    0, sizeof(ApplicationState));
    memset(&receivers, 0, sizeof(ReceiverState) * RECEIVER_COUNT);

    init_rtc();

    Wire.begin();

    init_rf();

    init_buttons();
    init_lcd();

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

    update_lcd();

    delay(50);
}

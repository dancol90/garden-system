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
#include "config.h"
#include "digit_font.h"

#define selected_receiver receivers[state.selected_recv]

// ############################################################################################

RF24 radio(RF24_CE, RF24_CSN);

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
}

void loop() {

    // Update time
    read_time();

    // Update button states
    update_buttons();

    if (state.menu_active) {
        update_menu();
    } else {
        // Update homescreen state
        update_homescreen();
    }

    delay(50);
}

/*void print_jobs() {
    for (byte a = 0; a < RECEIVER_COUNT; a++) {
        Serial.print(F("Receiver ")); Serial.print(a);
        if (receivers[a].active) Serial.print(F("[active]"));
        Serial.println(); 

        for (byte i = 0; i < 10; i++) {
            Serial.print(i); Serial.print(" - ");
            print_job(receivers[a].jobs[i]);
            Serial.println();
        }

        Serial.println(F("-------------------"));
    }
}


void print_job(Interval& t) {
    print_time(t.start);
    Serial.print(F(" -> "));
    print_time(t.end);    
}

void print_time(Time& t) {
    Serial.print(t.s.h);Serial.print(':');Serial.print(t.s.m);
   
}
*/

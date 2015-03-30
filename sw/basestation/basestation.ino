/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include <SPI.h>
#include <Wire.h>

#include <LightPCD8544.h>
#include "nRF24L01.h"
#include "RF24.h"

#include <MenuLib.h>
#include <NokiaLcdDrawer.h>

#include "structures.h"
#include "config.h"
#include "digit_font.h"

#define selected_recv receivers[state.selected_recv]

#define get_job(index) selected_recv.jobs[index]
#define set_job(index, job) selected_recv.jobs[index] = job

// ############################################################################################

RF24 radio(RF24_CE, RF24_CSN);

Time now;
ApplicationState state;

ReceiverState receivers[RECEIVER_COUNT];

LightPCD8544 lcd = LightPCD8544(LCD_DC, LCD_CS);
MenuController menu;

#include "menu_custom.h"

// ############################################################################################

void setup() {
    Serial.begin(9600);

    memset(&state, 0, sizeof(ApplicationState));
    
    // Clear data.
    // TODO: load from eeprom
    memset(&receivers, 0, sizeof(ReceiverState) * RECEIVER_COUNT);

    Wire.begin();

    init_rtc();

    init_rf();

    init_buttons();
    init_lcd();
    init_menu();
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

        // Draw homescreen
        draw_homescreen();
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
   
}*/

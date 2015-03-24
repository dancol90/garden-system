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

// ############################################################################################

Time now;
ApplicationState state;

LightPCD8544 lcd = LightPCD8544(LCD_DC, LCD_CS);
MenuController menu;

// ############################################################################################
void setup() {
    Serial.begin(9600);

    memset(&state, 0, sizeof(ApplicationState));

    Wire.begin();

    init_rtc();

    init_buttons();
    init_lcd();
    init_menu();

    //Serial.println("-----");
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

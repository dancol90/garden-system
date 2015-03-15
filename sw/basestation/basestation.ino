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

    memset(&state, 0, sizeof(state));

    Wire.begin();

    init_buttons();
    init_lcd();
    init_menu();
}

void loop() {

    // Update time
    read_time();

    // Update button states
    update_buttons();

    // Update homescreen state
    update_homescreen();

    // Draw homescreen
    draw_homescreen();

    /*if (is_pressed(BTN_UP))
        menu.prev();
    else if (is_pressed(BTN_DOWN))
        menu.next();
    else if (is_pressed(BTN_OK))
        menu.select();
    else if (is_pressed(BTN_BACK))
        menu.back();


    menu.draw();*/

    delay(10);
}

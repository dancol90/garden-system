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

LightPCD8544 lcd = LightPCD8544(LCD_DC, LCD_CS);
MenuController menu;

uint8_t backlight = 5, contrast = 5;

// ############################################################################################

void draw_selector(int x, int y) {
    lcd.drawHLine(x    , y    , 5, BLACK);
    lcd.drawHLine(x + 1, y + 1, 3, BLACK);
    lcd.drawHLine(x + 2, y + 2, 1, BLACK);
}

void draw_homescreen() {
    
    lcd.clearDisplay();

    read_time();

    draw_big_digit(4,          1, now.s.h   / 10);   
    draw_big_digit(6 + font_w, 1, now.s.h   % 10); 

    draw_big_digit(6 + 2*font_w, 1, '.');

    draw_big_digit(14 + 2*font_w, 1, now.s.m / 10);   
    draw_big_digit(16 + 3*font_w, 1, now.s.m % 10);

    lcd.setCursor(1, 26);
    lcd.print("Next: 15:00 \x1a 16:00");

    lcd.fillRect( 0, 39, 42, 9, BLACK);
    lcd.drawChar(18, 40, '1', WHITE);

    lcd.drawRect(42, 39, 42, 9, BLACK);
    lcd.drawChar(60, 40, '2', BLACK);


    draw_selector(18, 35);

    lcd.display();
}

// ############################################################################################

void setup() {
    Serial.begin(9600);

    Wire.begin();

    init_buttons();
    init_lcd();
    init_menu();
}

void loop() {

    draw_homescreen();

    return;

    update_buttons();

    if (is_pressed(0))
        menu.prev();
    else if (is_pressed(1))
        menu.next();
    else if (is_pressed(2))
        menu.select();
    else if (is_pressed(3))
        menu.back();


    menu.draw();
}

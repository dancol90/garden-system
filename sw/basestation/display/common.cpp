/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "display.h"

void draw_big_digit(uint8_t xPos, uint8_t yPos, uint8_t digit) {
    uint8_t x, y, b, i, r = row_size;
    
    if(digit == '.') {
        digit = 10;
        
        r = row_size / 2;
    }
    
    for(y = 0; y < font_h; y++) {
        for(x = 0; x < r; x++) {
            b = pgm_read_byte(numbers + char_size * digit + r * y + x);
            
            for(i = 0; i < 8; i++)
                lcd.drawPixel(xPos + x*8 + i, yPos + y, b & _BV(i));
        }
    }
}


void show_message(const __FlashStringHelper* msg) {
    lcd.clear();

    uint8_t y = (lcd.height() - 7) / 2;
    uint8_t x = (lcd.width() - lcd.getStringWidth(msg)) / 2;

    lcd.setCursor(x, y);
    lcd.print(msg);

    lcd.update();
}
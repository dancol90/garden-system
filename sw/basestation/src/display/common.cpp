/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "display.h"

// Draw a big digit. Used to draw time on homescreen
void display_draw_big_digit(uint8_t xPos, uint8_t yPos, uint8_t digit) {
    uint8_t x, y, b, i, r = row_size;
    
    // The separating dot is the tenth symbol in the font
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

// Show a centered message on screen.
void display_show_message(const __FlashStringHelper* msg) {
    lcd.clear();

    // Assuming that the font height is 7px
    uint8_t y = (lcd.height() - 7) / 2;
    uint8_t x = (lcd.width() - lcd.getStringWidth(msg)) / 2;

    lcd.setCursor(x, y);
    lcd.print(msg);

    lcd.update();
}

void display_draw_selector(int x, int y) {
    lcd.drawHLine(x    , y    , 5, BLACK);
    lcd.drawHLine(x + 1, y + 1, 3, BLACK);
    lcd.drawHLine(x + 2, y + 2, 1, BLACK);
}

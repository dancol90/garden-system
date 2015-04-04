/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

long lcd_bl_time;

void init_lcd() {
    // Setup LCD backlight pin
    pinMode(LCD_LED, OUTPUT);
    
    lcd.begin();

    state.lcd_settings_dirty = true;
}

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

void update_lcd() {
    if (state.lcd_settings_dirty) {
        start_backlight_timer();
        lcd.setContrast(40 + settings.contrast);

        state.lcd_settings_dirty = false;
    }

    if (lcd_bl_time && millis() - lcd_bl_time > 10000) {
        analogWrite(LCD_LED, 0);
        lcd_bl_time = 0;
    }

}

void start_backlight_timer() {
    lcd_bl_time = millis();
    
    analogWrite(LCD_LED, 12 * settings.backlight);
}

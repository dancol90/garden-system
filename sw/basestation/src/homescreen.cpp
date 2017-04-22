/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/ 

#include "homescreen.h"

uint8_t _rect_w = lcd.width() / RECEIVER_COUNT;

long _update_timer = 0;

void draw_homescreen() {
    
    lcd.clear();

    display_draw_big_digit(4,          1, rtc_now.time.hour   / 10);   
    display_draw_big_digit(6 + font_w, 1, rtc_now.time.hour   % 10); 

    display_draw_big_digit(6 + 2*font_w, 1, '.');

    display_draw_big_digit(14 + 2*font_w, 1, rtc_now.time.minute / 10);   
    display_draw_big_digit(16 + 3*font_w, 1, rtc_now.time.minute % 10);

#ifdef USE_SSD1306
    sprintf(job_name, "%s %d", rtc_get_dow_string(rtc_now), rtc_now.date.day);

    lcd.setCursor(86, 4);
    lcd.print(job_name);
    lcd.setCursor(86, 14);
    lcd.print(rtc_get_month_string(rtc_now));
#endif

#ifdef USE_WIFI
    // Draw WiFi signal bar
    
    byte signal = wifi_get_quality();

    if (signal != 0xFF) {
        for (uint8_t i = 0; i <= 3; i++) {
            if (signal > 25*i)
                lcd.drawVLine(120 + 2*i, 6 - 2*i , 2*i + 2, BLACK);
            else
                lcd.drawPixel(120 + 2*i, 8, BLACK);
        }
    }

#endif

    lcd.setCursor(24, 32);
    
    if (selected_receiver.current_job >= 0) {
        storage_get_job_str(selected_receiver.current_job, job_name);
        
        lcd.print(F("Now:  "));
        lcd.print(job_name);        
    } else if (selected_receiver.next_job >= 0) {
        storage_get_job_str(selected_receiver.next_job, job_name);
        
        lcd.print(F("Next: "));
        lcd.print(job_name);
    }

    uint8_t offset_x = 0, char_x, c;
    uint8_t offset_y = lcd.height() - 15;

    lcd.drawHLine(0, offset_y, lcd.width(), BLACK);

    for (uint8_t i = 0; i < RECEIVER_COUNT; i++) {
        if (receivers[i].active)
            lcd.fillRect(offset_x, offset_y, _rect_w, 15, BLACK);
        else if (i > 0)
            lcd.drawVLine(offset_x, offset_y, 15, BLACK);

        c = '1' + i;
        char_x = offset_x + (_rect_w - lcd.getCharWidth(c)) / 2;

        lcd.drawChar(char_x, offset_y + 4, c, !receivers[i].active); 

        if (receivers[i].got_rf) {
            //display_draw_selector(offset_x, offset_y - 4);
            lcd.fillRect(char_x - 5, offset_y + 6, 3, 3, !receivers[i].active);

            receivers[i].got_rf = false;
        }

        offset_x += _rect_w;
    }

    display_draw_selector(_rect_w / 2 + _rect_w * state.selected_recv - 3, offset_y - 4);

    lcd.update();
}

void home_update() {
    bool changed = true;

    if (input_is_pressed(BTN_UP))
        state.selected_recv = (state.selected_recv == 0) ? (RECEIVER_COUNT - 1) : (state.selected_recv - 1);
    else if (input_is_pressed(BTN_DOWN))
        state.selected_recv = (state.selected_recv + 1) % RECEIVER_COUNT;
    else if (input_is_pressed(BTN_OK))
        menu_enter();
    else
        changed = false;

    if (millis() - _update_timer > 300) {
        changed = true;

        _update_timer = millis();
    }

    if (changed && !state.menu_active)
        // Draw homescreen
        draw_homescreen();
}

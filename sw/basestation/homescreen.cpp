/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/ 

#include "homescreen.h"

uint8_t rect_w = lcd.width() / RECEIVER_COUNT;

long w = 0;

void draw_selector(int x, int y) {
    lcd.drawHLine(x    , y    , 5, BLACK);
    lcd.drawHLine(x + 1, y + 1, 3, BLACK);
    lcd.drawHLine(x + 2, y + 2, 1, BLACK);
}

void draw_homescreen() {
    
    lcd.clear();

    draw_big_digit(4,          1, now.time.hour   / 10);   
    draw_big_digit(6 + font_w, 1, now.time.hour   % 10); 

    draw_big_digit(6 + 2*font_w, 1, '.');

    draw_big_digit(14 + 2*font_w, 1, now.time.minute / 10);   
    draw_big_digit(16 + 3*font_w, 1, now.time.minute % 10);

#ifdef USE_SSD1306
    sprintf(job_name, "%s %d", get_dow_string(now), now.date.day);

    lcd.setCursor(86, 4);
    lcd.print(job_name);
    lcd.setCursor(86, 14);
    lcd.print(get_month_string(now));
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
        get_job_string(selected_receiver.current_job, job_name);
        
        lcd.print(F("Now:  "));
        lcd.print(job_name);        
    } else if (selected_receiver.next_job >= 0) {
        get_job_string(selected_receiver.next_job, job_name);
        
        lcd.print(F("Next: "));
        lcd.print(job_name);
    }

    uint8_t offset_x = 0, char_x, c;
    uint8_t offset_y = lcd.height() - 15;

    lcd.drawHLine(0, offset_y, lcd.width(), BLACK);

    for (uint8_t i = 0; i < RECEIVER_COUNT; i++) {
        if (receivers[i].active)
            lcd.fillRect(offset_x, offset_y, rect_w, 15, BLACK);
        else if (i > 0)
            lcd.drawVLine(offset_x, offset_y, 15, BLACK);

        c = '1' + i;
        char_x = offset_x + (rect_w - lcd.getCharWidth(c)) / 2;

        lcd.drawChar(char_x, offset_y + 4, c, !receivers[i].active); 

        if (receivers[i].got_rf) {
            //draw_selector(offset_x, offset_y - 4);
            lcd.fillRect(char_x - 5, offset_y + 6, 3, 3, !receivers[i].active);

            receivers[i].got_rf = false;
        }

        offset_x += rect_w;
    }

    draw_selector(rect_w / 2 + rect_w * state.selected_recv - 3, offset_y - 4);

    lcd.update();
}

void update_homescreen() {
    bool changed = true;

    if (is_pressed(BTN_UP))
        state.selected_recv = (state.selected_recv == 0) ? (RECEIVER_COUNT - 1) : (state.selected_recv - 1);
    else if (is_pressed(BTN_DOWN))
        state.selected_recv = (state.selected_recv + 1) % RECEIVER_COUNT;
    else if (is_pressed(BTN_OK))
        enter_menu();
    else
        changed = false;

    if (millis() - w > 300) {
        changed = true;

        w = millis();
    }

    if (changed && !state.menu_active)
        // Draw homescreen
        draw_homescreen();
}

/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/ 

void draw_selector(int x, int y) {
    lcd.drawHLine(x    , y    , 5, BLACK);
    lcd.drawHLine(x + 1, y + 1, 3, BLACK);
    lcd.drawHLine(x + 2, y + 2, 1, BLACK);
}

void draw_homescreen() {
    
    lcd.clear();

    draw_big_digit(4,          1, now.s.h   / 10);   
    draw_big_digit(6 + font_w, 1, now.s.h   % 10); 

    draw_big_digit(6 + 2*font_w, 1, '.');

    draw_big_digit(14 + 2*font_w, 1, now.s.m / 10);   
    draw_big_digit(16 + 3*font_w, 1, now.s.m % 10);

    lcd.setCursor(1, 26);
    lcd.print(F("Next: 15:00 \x1a 16:00"));

    uint8_t rect_w = LCD_WIDTH / RECEIVER_COUNT;
    uint8_t char_x = (rect_w / 2) - 3;
    uint8_t offset = 0;

    lcd.drawHLine(0, 39, LCD_WIDTH, BLACK);

    for (uint8_t i = 0; i < RECEIVER_COUNT; i++) {
        if (receivers[i].active)
            lcd.fillRect(offset, 39, rect_w, 9, BLACK);
        else if (i > 0)
            lcd.drawVLine(offset, 39, 9, BLACK);

        lcd.drawChar(offset + char_x, 40, '1' + i, !receivers[i].active); 

        offset += rect_w;       
    }

    draw_selector(char_x + rect_w * state.selected_recv, 35);

    lcd.update();
}

void update_homescreen() {
    if (is_pressed(BTN_UP))
        state.selected_recv = (state.selected_recv == 0) ? (RECEIVER_COUNT - 1) : (state.selected_recv - 1);
    else if (is_pressed(BTN_DOWN))
        state.selected_recv = (state.selected_recv + 1) % RECEIVER_COUNT;
    else if (is_pressed(BTN_OK))
        state.menu_active = true;
}
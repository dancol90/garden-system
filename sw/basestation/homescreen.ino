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

    lcd.fillRect( 0, 39, 42, 9, BLACK);
    lcd.drawChar(18, 40, '1', WHITE);

    lcd.drawRect(42, 39, 42, 9, BLACK);
    lcd.drawChar(60, 40, '2', BLACK);

    draw_selector(18 + 42 * state.receiver_no, 35);

    lcd.update();
}

void update_homescreen() {
    if (is_pressed(BTN_UP))
        state.receiver_no = abs( (state.receiver_no - 1) % RECEIVER_COUNT );
    else if (is_pressed(BTN_DOWN))
        state.receiver_no = (state.receiver_no + 1) % RECEIVER_COUNT;
    else if (is_pressed(BTN_OK))
        state.menu_active = true;
}
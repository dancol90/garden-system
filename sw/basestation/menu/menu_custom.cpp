/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "menu_custom.h"

char job_name[14];

const char* JobEntry::getText() {
    get_job_string(this->data, job_name);

    return job_name;
}

bool JobEntry::isEnabled() { return get_job(this->data).enabled; }



const char* TimeSelector::getSecondaryText() {
    // TODO: this give errors on PSTR...why?
    //snprintf_P(valueStr, 10, PSTR("<%2d:%02d>"), variable.hour, variable.minute);
    snprintf(valueStr, 10, "<%2d:%02d>", variable.hour, variable.minute);


    return valueStr;
}

bool TimeSelector::activate() {
    if (this->callback) this->callback(true);

    oldValue = variable;

    // Select Hours first
    stage = 0;

    return true;
}

void TimeSelector::deactivate(){ variable = oldValue; }

void TimeSelector::doNext() {
    if (stage) {
        // Minutes
        variable.minute = (variable.minute + 1) % 60;
    } else {
        // Hours
        variable.hour = (variable.hour + 1) % 24;
    }
}

void TimeSelector::doPrev() {
    if (stage) {
        // Minutes
        variable.minute = variable.minute == 0 ? 59 : variable.minute - 1;
    } else {
        // Hours
        variable.hour = variable.hour == 0 ? 23 : variable.hour - 1;
    }
}

MenuItem* TimeSelector::action() {
    if (stage == 0) {
        stage++;
        
        return NULL;
    } else {
        if (this->callback) this->callback(false);

        return this->getParent();
    }
}

/****************************************/

bool PasswordInput::activate() {
    memset(buffer, 0, 21);
    sel_i = 0;
    buffer[0] = 'a';
    return true;
}

void PasswordInput::doNext() {
    buffer[sel_i] = ' ' + (buffer[sel_i] - ' ' + 1) % 96; 
}

void PasswordInput::doPrev() {
    buffer[sel_i] = ' ' + (buffer[sel_i] - ' ' + 95) % 96;
}

MenuItem* PasswordInput::action() {
    if (buffer[sel_i] == 0x7F) {
        // Remove the last special character
        buffer[sel_i] = 0;

        // Done
        if (this->callback) this->callback((const char*)buffer);
        return this->getParent();
    } else {
        // Jump to the next char
        if(sel_i < 19) { 
            sel_i++;
            buffer[sel_i] = 'a';
        }
        return NULL;
    }
}

bool PasswordInput::back() {
    // Do not block the action if the first char is selected
    if (sel_i == 0) {
        return true;
    } else {
        buffer[sel_i] = 0;
        sel_i--;
        
        return false;
    }
}

/****************************************/

void CustomLcdDrawer::drawOther(MenuItem* item) {
    const char type = item->getTypeId();

    if (type == 'd') {
        TimeSelector* t = (TimeSelector*)item;

        // Draw title
        drawCenterText(t, 0);
        
        // Draw a rect around the value
        lcd.drawRect(RECT1_X, RECT_Y, RECT_W, RECT_H, 1);
        lcd.drawRect(RECT2_X, RECT_Y, RECT_W, RECT_H, 1);

        // Some decoration
        int x = (t->getStage() ? RECT2_X : RECT1_X);
        lcd.drawChar(x + 3,          TEXT_Y, '<', BLACK);
        lcd.drawChar(x + RECT_W - 6, TEXT_Y, '>', BLACK);

        // Guess the x pos to center the value number
        drawCenterNumber(t->getValue().hour, TEXT_Y, RECT1_X, RECT1_X + RECT_W);
        drawCenterNumber(t->getValue().minute, TEXT_Y, RECT2_X, RECT2_X + RECT_W);
    } else if (type == 'p') {
        PasswordInput* p = (PasswordInput*)item;

        // Draw title
        drawCenterText(p, 0);

        lcd.setCursor(20, TEXT_Y);

        const char* pass = p->getValue();
        uint8_t i = 0, w;

        while(*pass != 0) {
            if (i == p->getActiveChar()) {
                w = lcd.getCharWidth(*pass);

                lcd.drawHLine(lcd.getCursorX(), TEXT_Y - 2, w, BLACK);
                lcd.drawHLine(lcd.getCursorX(), TEXT_Y + 9, w, BLACK);
            }

            if (*pass == 0x7f)
                lcd.print("<done>");
            else
                lcd.print(*pass);

            pass++;
            i++;
        }
    }
} 
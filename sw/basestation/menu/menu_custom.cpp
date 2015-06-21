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


void CustomLcdDrawer::drawOther(MenuItem* item) {
    if (item->getTypeId() == 'd') {
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
    }
} 
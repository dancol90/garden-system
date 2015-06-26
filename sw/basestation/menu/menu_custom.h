/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __menu_custom_h__
#define __menu_custom_h__

#include "menu.h"
#include <LcdDrawer.h>

// Job name is generated on the fly, so I can use a global buffer to spare a LOT of RAM
extern char job_name[];

class JobEntry : public ParamAction<uint8_t> {
    public:
        JobEntry(MenuItem* parent, ActionCallback callback, uint8_t index) : ParamAction(parent, (char*)NULL, callback, index) {
            this->is_flash = false;
        };

        const char* getText();

        bool isEnabled();
};

class TimeSelector : public MenuItem {
    public:
        typedef void(*TimeSelectedCallback)(bool);

        TimeSelector(MenuItem* parent, const FlashString* text, Time& variable, TimeSelectedCallback callback = NULL) : MenuItem(parent, text), variable(variable), callback(callback) {};

        Time getValue() { return variable; }

        // Return which element is being modified (1 for minute, 0 for hours)
        uint8_t getStage() { return stage; }
        char getTypeId() { return 'd'; };

        const char* getSecondaryText();

        bool activate();
        void deactivate();

        void doNext();

        void doPrev();

        MenuItem* action();

    private:
        Time& variable;
        Time  oldValue;

        uint8_t stage;
    
        char valueStr[10];

        TimeSelectedCallback callback;
};

// Leave this line commented to use library's defaults
#undef RECT_W
#undef RECT_Y
#define RECT_W 31
//#define RECT_H 14
#define RECT_Y 25

#define RECT1_X (lcd.width() - 2*RECT_W) / 3
#define RECT2_X 2*RECT1_X + RECT_W
//#define TEXT_Y  RECT_Y + (RECT_H - 7) / 2

class CustomLcdDrawer : public LcdDrawer {
    protected:
        void drawOther(MenuItem* item);
    public:
        CustomLcdDrawer(LightLCD& lcd) : LcdDrawer(lcd, 4) {}
};


#endif

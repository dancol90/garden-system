/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include <LightLCD.h>
#include "structures.h"

// Job name is generated on the fly, so I can use a global buffer to spare a LOT of RAM
char job_name[14];

class JobEntry : public ParamAction<uint8_t> {
    public:
        JobEntry(MenuItem* parent, ActionCallback callback, int index) : ParamAction(parent, NULL, callback, index) {
            this->is_flash = 0;
        }

        const char* getText() {
            get_job_string(this->data, job_name);

            return job_name;
        }

        bool isEnabled() { return get_job(this->data).enabled; }
};

class TimeSelector : public MenuItem {
    public:
        typedef void(*TimeSelectedCallback)(bool);


        TimeSelector(MenuItem* parent, const FlashString* text, Time& variable, TimeSelectedCallback callback = NULL) : MenuItem(parent, text), variable(variable), callback(callback) {};

        Time getValue() { return variable; }

        // Return which element is being modified (1 for minute, 0 for hours)
        uint8_t getStage() { return stage; }

        char getTypeId() { return 'd'; };

        const char* getSecondaryText() {
            sprintf_P(valueStr, PSTR("<%2d:%02d>"), variable.s.h, variable.s.m);

            return valueStr;
        }

        bool activate() {
            if (this->callback) this->callback(true);

            oldValue = variable;

            // Select Hours first
            stage = 0;

            return true;
        }
        void deactivate(){ variable = oldValue; }

        void doNext() {
            if (stage) {
                // Minutes
                variable.s.m = (variable.s.m + 1) % 60;
            } else {
                // Hours
                variable.s.h = (variable.s.h + 1) % 24;
            }
        }

        void doPrev() {
            if (stage) {
                // Minutes
                variable.s.m = variable.s.m == 0 ? 59 : variable.s.m - 1;
            } else {
                // Hours
                variable.s.h = variable.s.h == 0 ? 23 : variable.s.h - 1;
            }
        }

        MenuItem* action() {
            if (stage == 0)
                stage++;
            else {
                if (this->callback) this->callback(false);

                return this->getParent();
            }
        }

    private:
        Time& variable;
        Time  oldValue;

        uint8_t stage;
    
        char valueStr[10];

        TimeSelectedCallback callback;
};

// Leave this line commented to use library's defaults
#define RECT_W 31
//#define RECT_H 14
//#define RECT_Y 18

#define RECT1_X (lcd.width() - 2*RECT_W) / 3
#define RECT2_X 2*RECT1_X + RECT_W
//#define TEXT_Y  RECT_Y + (RECT_H - 7) / 2

class CustomLcdDrawer : public LcdDrawer {
    protected:
        void drawOther(MenuItem* item) {
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
                drawCenterNumber(t->getValue().s.h, TEXT_Y, RECT1_X, RECT1_X + RECT_W);
                drawCenterNumber(t->getValue().s.m, TEXT_Y, RECT2_X, RECT2_X + RECT_W);
            }
        } 

    public:
        CustomLcdDrawer(LightLCD& lcd) : LcdDrawer(lcd) {}
};

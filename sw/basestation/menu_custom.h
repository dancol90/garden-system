/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

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
        typedef void(*TimeSelectedCallback)(void);


        TimeSelector(MenuItem* parent, const __FlashStringHelper* text, Time& variable, TimeSelectedCallback callback = NULL) : MenuItem(parent, text), variable(variable), callback(callback) {};

        Time getValue() { return variable; }

        // Return which element is being modified (1 for minute, 0 for hours)
        uint8_t getStage() { return stage; }

        char getTypeId() { return 'd'; };

        const char* getSecondaryText() {
            sprintf_P(valueStr, PSTR("<%2d:%02d>"), variable.s.h, variable.s.m);

            return valueStr;
        }

        bool activate() {
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
                if (this->callback) this->callback();

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

class LcdDrawer : public NokiaLcdDrawer {
    protected:
        void drawOther(MenuItem* item) {
            if (item->getTypeId() == 'd') {
                TimeSelector* t = (TimeSelector*)item;

                // Draw title
                drawCenterText(t, 0);
                
                // Draw a rect around the value
                lcd.drawRect( 7, 18, 31, 14, 1);
                lcd.drawRect(46, 18, 31, 14, 1);
             
                // Some decoration
                lcd.drawChar(11 + 39 * t->getStage(), 21, '<', BLACK);
                lcd.drawChar(31 + 39 * t->getStage(), 21, '>', BLACK);

                // Guess the x pos to center the value number
                drawCenterNumber(t->getValue().s.h, 21,  7, 38);
                drawCenterNumber(t->getValue().s.m, 21, 46, 77);
                
            }
        } 

    public:
        LcdDrawer(LightPCD8544& lcd) : NokiaLcdDrawer(lcd) {}
};
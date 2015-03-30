#include "structures.h"

class TimeSelector : public MenuItem {
    private:
        Time& variable;
        Time  oldValue;

        uint8_t stage;
    
        char valueStr[10];
    public:
        TimeSelector(MenuItem* parent, const __FlashStringHelper* text, Time& variable) : MenuItem(parent, text), variable(variable) {};

        Time getValue() { return variable; }

        // Return which element is being modified (1 for minute, 0 for hours)
        uint8_t getStage() { return stage; }

        char getTypeId() { return 'd'; };

        const char* getSecondaryText() {
            sprintf(valueStr, "<%d:%d>", variable.s.h, variable.s.m);

            return valueStr;
        }

        int activate() {
            oldValue = variable;

            // Select Hours first
            stage = 0;

            return 1;
        }

        int deactivate(){
            variable = oldValue;

            return 1;
        }

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
            else
                return this->getParent();
        }
};

class LcdDrawer : public NokiaLcdDrawer {
    protected:
        void drawOther(MenuItem* item) {
            Serial.println("DrawOther!!!");

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
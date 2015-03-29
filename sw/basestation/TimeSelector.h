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

        }

        void doPrev() {

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
                //lcd.drawLine(0, 8, LCD_WIDTH, 8, 1) ;
                
                // Draw a rect around the value
                lcd.drawRect(8, 18, 68, 14, 1) ;
             
                // Some decoration
                lcd.setCursor(11, 21);
                lcd.print('<');
                
                // Guess the x pos to center the value number
                drawCenterNumber(t->getValue().s.h, 21);
                
                // Still some decorations
                lcd.setCursor(69, 21);
                lcd.println('>');
            }
        } 

    public:
        LcdDrawer(LightPCD8544& lcd) : NokiaLcdDrawer(lcd) {}
};
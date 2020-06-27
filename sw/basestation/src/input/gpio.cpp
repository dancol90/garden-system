/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "input.h"

#ifdef USE_INPUT_GPIO

ButtonState buttons[4];

inline bool timed_out(int i, long interval) {
    return millis() - buttons[i].timer > interval;
}

void input_init() {
    for (int i = 0; i < 4; i++) {
        pinMode(button_pins[i], INPUT_PULLUP);
        memset(&(buttons[i]), 0, sizeof(ButtonState));
    }
}

bool input_update() {
    bool button_pressed = false;

    for (int i = 0; i < 4; i++) {
        // Reset state
        buttons[i].changed = false;
        
        uint8_t r = !digitalRead(button_pins[i]);

        if (buttons[i].read != r) {
            // The input line has changed.

            // Save what has been just read
            buttons[i].read = r;
            // Start counting to let the line stabilize
            buttons[i].timer = millis();
            // The line is in a unstable state
            buttons[i].dirty = true;

        } else if (buttons[i].dirty) {
            // Here the input line hasn't changed, but it did a moment ago.
            // It could still be in an unstable state.

            if (timed_out(i, button_timer_interval)) {
                buttons[i].dirty = false;
                // Restart timer, this time to keep track of rebouncing
                buttons[i].timer = millis();
            
                if (buttons[i].state != r) {
                    buttons[i].state = r;
                    buttons[i].changed = true;

                    button_pressed = true;
                }
            }

        } else if (buttons[i].state && (i == BTN_UP || i == BTN_DOWN)) {

            if (timed_out(i, button_rebounce_interval)) {
                // The input is stable and in "pressed" state,
                // and the rebounce timer has timed out.

                // Do not change the state, but mark the button has changed.
                buttons[i].changed = true;
                // Restart the timer
                buttons[i].timer = millis();

                button_pressed = true;
            }
        }
    }

    return button_pressed;
}

bool input_is_pressed(uint8_t i) { return buttons[i].changed &&  buttons[i].state; }
bool input_released  (uint8_t i) { return buttons[i].changed && !buttons[i].state; }

#endif
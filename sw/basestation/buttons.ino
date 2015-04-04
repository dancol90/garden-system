/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

ButtonState buttons[4];

void init_buttons() {
    for (int i = 0; i < 4; i++) {
        pinMode(button_pins[i], INPUT_PULLUP);

        memset(&(buttons[i]), 0, sizeof(ButtonState));
    }
}

bool update_buttons() {
    bool button_pressed = false;

    for (int i = 0; i < 4; i++) {
        // Reset state
        buttons[i].changed = false;
        
        uint8_t r = !digitalRead(button_pins[i]);

        if (buttons[i].read != r) {
            buttons[i].read = r;
            buttons[i].timer = millis();
        } else if (millis() - buttons[i].timer > button_timer_interval) {
            if (buttons[i].state != r) {
                buttons[i].state = r;
                buttons[i].changed = true;

                button_pressed = true;
            }
        }
    }

    return button_pressed;
}

bool is_pressed(uint8_t i) { return buttons[i].changed &&  buttons[i].state; }
bool   released(uint8_t i) { return buttons[i].changed && !buttons[i].state; }

// ############################################################################################

const uint8_t button_timer_interval = 15;
const uint8_t button_analog_threshold = 100;

// ############################################################################################


uint8_t button_pins[] = { A2, A1, 4, 5 };
ButtonState buttons[4];

void init_buttons() {
    for (int i = 0; i < 4; i++) {
        pinMode(button_pins[i], INPUT_PULLUP);

        memset(&(buttons[i]), 0, sizeof(ButtonState));
    }
}

void update_buttons() {
    for (int i = 0; i < 4; i++) {
        // Reset state
        buttons[i].changed = FALSE;
        
        uint8_t r = !digitalRead(button_pins[i]);

        if (buttons[i].read != r) {
            buttons[i].read = r;
            buttons[i].timer = millis();
        } else if (millis() - buttons[i].timer > button_timer_interval) {
            if (buttons[i].state != r) {
                buttons[i].state = r;
                buttons[i].changed = TRUE;
            }
        }
    }
}

bool is_pressed(uint8_t i) { return buttons[i].changed &&  buttons[i].state; }
bool   released(uint8_t i) { return buttons[i].changed && !buttons[i].state; }

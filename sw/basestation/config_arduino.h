/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

const uint8_t LCD_DC  = 8;
const uint8_t LCD_CS  = 9;
const uint8_t LCD_LED = 6;

const uint8_t RF24_CE  = 10;
const uint8_t RF24_CSN = A0;

const uint8_t  RF24_PAYLOAD = sizeof(Packet);
const uint64_t RF24_BASE_PIPE = 0xF0F0F0F000LL;

const uint8_t RECEIVER_COUNT = 2;

const int     button_timer_interval = 15;
const int     button_rebounce_interval = 200;
const uint8_t button_analog_threshold = 100;

const uint8_t button_pins[] = { A2, A1, 4, 5 };

enum Button {
	BTN_UP,
	BTN_DOWN,
	BTN_OK,
	BTN_BACK
};


const uint8_t jobs_count = 10;
const uint16_t eeprom_settings_start = 0x01;
const uint16_t eeprom_jobs_start     = 0x0100;

const uint16_t eeprom_jobs_size = sizeof(Interval) * jobs_count;
const uint16_t eeprom_strings_start = eeprom_jobs_start + eeprom_jobs_size * RECEIVER_COUNT;

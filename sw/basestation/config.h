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

const uint64_t RF24_OUT_PIPE = 0xF0F0F0F0E1LL;
const uint64_t RF24_IN_PIPE  = 0xF0F0F0F0D2LL;

const uint8_t RECEIVER_COUNT = 2;

const uint8_t button_timer_interval = 15;
const uint8_t button_analog_threshold = 100;

const uint8_t button_pins[] = { A2, A1, 4, 5 };

enum Button {
	BTN_UP,
	BTN_DOWN,
	BTN_OK,
	BTN_BACK
};
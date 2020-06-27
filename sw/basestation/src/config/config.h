/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __config_h__
#define __config_h__

#include <Arduino.h>

#define USE_SSD1306
//#define USE_PCD8544

//#define USE_DS3231_RTC
#define USE_SW_RTC

//#define USE_INPUT_MPR121
#define USE_INPUT_GPIO

#define USE_WIFI

// ------------------------------------------------------

const int TIMEZONE = +2;
static const char* NTP_SERVER_HOST = "ntp1.inrim.it";

// ------------------------------------------------------

const uint8_t SSD1306_SDA_PIN = D1;
const uint8_t SSD1306_SCL_PIN = D2;

// ------------------------------------------------------

const uint8_t RF24_CE  = 15;
const uint8_t RF24_CSN = 0;

const uint64_t RF24_BASE_PIPE = 0xF0F0F0F000LL;

const uint8_t RECEIVER_COUNT = 2;

// ------------------------------------------------------

enum Button {
	BTN_UP,
	BTN_DOWN,
	BTN_OK,
	BTN_BACK
};

#ifdef USE_INPUT_GPIO
const uint8_t button_pins[] = { 12, 13, 14, 15 };
const int     button_timer_interval = 15;
const int     button_rebounce_interval = 200;
#endif

// ------------------------------------------------------

const uint8_t jobs_count = 10;
const uint16_t eeprom_settings_start = 0x01;
const uint16_t eeprom_jobs_start     = 0x0010;

#endif
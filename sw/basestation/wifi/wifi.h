/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __wifi_h
#define __wifi_h

#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>

#include "../config/config.h"
//#include "../settings.h"
#include "../rtc/rtc.h"


void wifi_init();
void wifi_update();
byte wifi_get_quality();

unsigned long ntp_get_timestamp();

#endif
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


void wifi_init();
byte wifi_get_quality();


#endif
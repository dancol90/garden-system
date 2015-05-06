/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#if defined(AVR)
	#include "config/config_arduino.h"
#elif defined(ESP8266)
	#include "config/config_esp8266.h"
#else
	#error "No compatible platform defined. Check your settings."
#endif
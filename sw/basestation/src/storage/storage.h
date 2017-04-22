/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __storage_h__
#define __storage_h__

#include <Arduino.h>
#include <EEPROM.h>

#include "../config/config.h"
#include "../rtc/rtc.h"

Interval storage_get_job_i(byte recv, byte index);
void storage_get_job_str_i(byte recv, byte index, char* buff);
void storage_save_job_i(byte recv, byte index, Interval job);

Interval storage_get_job(byte index);
void storage_get_job_str(byte index, char* buff);
void storage_save_job(byte index, Interval job);

void storage_save_settings();
void storage_load_settings();

void storage_format();
void storage_init();

#endif
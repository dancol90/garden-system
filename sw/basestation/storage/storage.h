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

const uint16_t eeprom_jobs_size = sizeof(Interval) * jobs_count;
const uint16_t eeprom_strings_start = eeprom_jobs_start + eeprom_jobs_size * RECEIVER_COUNT;

#define eeprom_job_address(recv, index) eeprom_jobs_start + eeprom_jobs_size * recv + sizeof(Interval) * index
#define eeprom_job_string_address(recv, index) eeprom_strings_start + 14 * (jobs_count * recv + index)

Interval get_recv_job(byte recv, byte index);
void get_recv_job_string(byte recv, byte index, char* buff);
void save_recv_job(byte recv, byte index, Interval job);

Interval get_job(byte index);
void get_job_string(byte index, char* buff);
void save_job(byte index, Interval job);

void save_settings();
void load_settings();

void format_eeprom();
void init_eeprom();

#endif
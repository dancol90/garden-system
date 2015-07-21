/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "storage.h"
 
#ifdef ESP8266
  #define EEPROM_update EEPROM.write
  #define EEPROM_commit EEPROM.commit
  #define EEPROM_begin  EEPROM.begin
#else
  #define EEPROM_update EEPROM.update
  #define EEPROM_commit
  #define EEPROM_begin
#endif

const uint16_t eeprom_jobs_size = sizeof(Interval) * jobs_count;
const uint16_t eeprom_strings_start = eeprom_jobs_start + eeprom_jobs_size * RECEIVER_COUNT;

#define eeprom_job_address(recv, index) eeprom_jobs_start + eeprom_jobs_size * recv + sizeof(Interval) * index
#define eeprom_job_string_address(recv, index) eeprom_strings_start + 14 * (jobs_count * recv + index)

Interval storage_get_job_i(byte recv, byte index) {
    Interval job;

    EEPROM.get(eeprom_job_address(recv, index), job);

    return job;
}

void storage_get_job_str_i(byte recv, byte index, char* buff) {
    // Can't use EEPROM.get here because it doesn't works with arrays of chars (sizeof(char*) = size of the pointer, not the content)
    for (byte i = 0; i < 14; i++) {
        buff[i] = EEPROM.read(eeprom_job_string_address(recv, index) + i);
    }

    buff[13] = 0x00;
}

void storage_save_job_i(byte recv, byte index, Interval job) {
    EEPROM.put(eeprom_job_address(recv, index), job);

    // Generate string
    char job_name[14];

    snprintf_P(job_name, 14,
        PSTR("%2d:%02d \x1a %2d:%02d"),
        job.start.hour, job.start.minute,
        job.end.hour,   job.end.minute
    );

    // Can't use EEPROM.put here because it doesn't works with arrays of chars (sizeof(char*) = size of the pointer, not the content)
    for (byte i = 0; i < 14; i++) {
        EEPROM_update(eeprom_job_string_address(recv, index) + i, job_name[i]);
    }
}

void storage_save_settings() { EEPROM.put(eeprom_settings_start, settings); }
void storage_load_settings() { EEPROM.get(eeprom_settings_start, settings); }

void storage_format() {
    Interval empty_job;

    memset(&empty_job, 0, sizeof(Interval));

    for (byte r = 0; r < RECEIVER_COUNT; r++) {
        for (byte j = 0; j < jobs_count; j++) {
            storage_save_job_i(r, j, empty_job);
        }
    }
}

void storage_init() {
    EEPROM_begin(500);

    if (EEPROM.read(0) != 0xDC) {
        storage_format();

        EEPROM.write(0, 0xDC);
    }
}


Interval storage_get_job(byte index)                { return storage_get_job_i(state.selected_recv, index); }
void storage_get_job_str(byte index, char* buff) { storage_get_job_str_i(state.selected_recv, index, buff); }
void storage_save_job(byte index, Interval job)     { storage_save_job_i(state.selected_recv, index, job); }

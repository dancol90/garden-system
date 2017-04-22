/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/
#include "wifi.h"

#ifdef USE_WIFI

extern "C" {
#include <sntp.h>
}

const long JAN_1_2010 = 1262304000UL;

unsigned long ntp_get_timestamp() {
    sntp_stop();
    sntp_setservername(0, (char*)NTP_SERVER_HOST);

    sntp_set_timezone(TIMEZONE);

    sntp_init();

    // Wait a moment to let the sntp system to start.
    delay(200);

    // Espressif API uses the GMT+8 timezone
    long current_stamp = sntp_get_current_timestamp();

    // Compensate for the wrong timezone and change reference year to 2010
    return current_stamp - JAN_1_2010;
}

#endif
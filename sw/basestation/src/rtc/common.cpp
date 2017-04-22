/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "rtc.h"
#include "strings.h"

const char*   rtc_get_dow_string(DateTime t) { return rtc_str_days[t.date.dow]; }
const char* rtc_get_month_string(DateTime t) { return rtc_str_months[t.date.month - 1]; }

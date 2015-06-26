/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "rtc.h"
#include "strings.h"

const char*   get_dow_string(DateTime t) { return days[t.date.dow]; }
const char* get_month_string(DateTime t) { return months[t.date.month - 1]; }

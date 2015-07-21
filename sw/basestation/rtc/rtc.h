/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __rtc_h__
#define __rtc_h__

#include <Arduino.h>

#include "../config/config.h"
#include "../settings.h"

struct Time {
    uint8_t minute;
    uint8_t hour;
    uint8_t second;

    long stamp() const { return (minute + (hour << 8)); }

    bool operator==(const Time& a) const { return (minute == a.minute && hour == a.hour); }

    bool operator>(const Time& a) const { return stamp() > a.stamp(); }
    bool operator<(const Time& a) const { return stamp() < a.stamp(); }

    bool operator>=(const Time& a) const { return stamp() >= a.stamp(); }
    bool operator<=(const Time& a) const { return stamp() <= a.stamp(); }

    bool operator>(const long& a) const { return stamp() > a; }
    bool operator<(const long& a) const { return stamp() < a; }

};

struct Date {
    uint8_t day;
    uint8_t month;
    int     year;

    uint8_t dow;
    bool    leap;
};

struct DateTime {
    Time time;
    Date date;
};

struct Interval {
    Time start;
    Time end;

    bool enabled : 1;
};

extern DateTime rtc_now;

void rtc_init();
void rtc_update();

void rtc_write_time(DateTime t);
void rtc_write_time_from_timestamp(long timestamp);

const char*   rtc_get_dow_string(DateTime t);
const char* rtc_get_month_string(DateTime t);

#endif
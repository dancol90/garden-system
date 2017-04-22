/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/ 

#include "rtc.h"

#ifdef USE_SW_RTC

DateTime rtc_now;

// Day per month
const short _dpm[]  = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// Sakamoto's costants for DOW calculation
const short _dow_const[]   = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

long _last_millis, _buffer_millis;

bool is_leap(int year) { 
    return (rtc_now.date.year % 4)   == 0 &&
           (rtc_now.date.year % 100) != 0 ||
           (rtc_now.date.year % 400) == 0;
}

void rtc_write_time(DateTime t) { rtc_now = t; }

void rtc_write_time_from_timestamp(long timestamp) {
    // Get only today's seconds
    int since_midnight = timestamp % 86400UL;

    // Calculate time info.
    rtc_now.time.hour   =  since_midnight / 3600;
    rtc_now.time.minute = (since_midnight % 3600) / 60;
    rtc_now.time.second =  since_midnight % 60;

    // Get how many days passed since 1 Jen 2010.
    int days = timestamp / 86400UL;

    // As said, timestamp = 0 => 1 Jen 2010
    rtc_now.date.year = 2010;

    // Starting from 2010, remove every year's days from the count, to get correct year.
    while (days >= 365 + is_leap(rtc_now.date.year)) {
        days -= 365 + is_leap(rtc_now.date.year);

        rtc_now.date.year++;
    }

    // Is the year we just found leap?
    rtc_now.date.leap = is_leap(rtc_now.date.year);

    // Start from Jen, in base 0
    rtc_now.date.month = 0;

    // As for the year, remove month's day from the count, to get correct day.
    while (days >= _dpm[rtc_now.date.month]) {
        days -= _dpm[rtc_now.date.month] + (rtc_now.date.leap && (rtc_now.date.month == 1));

        rtc_now.date.month++;
    }

    // Bring back the month to base 1
    rtc_now.date.month++;

    // Do the same for days
    rtc_now.date.day = days + 1;

    // Sakamoto algorithm for Day Of Week
    int y = rtc_now.date.year - (rtc_now.date.month < 3);
    rtc_now.date.dow = (y + y/4 - y/100 + y/400 + _dow_const[rtc_now.date.month-1] + rtc_now.date.day) % 7;
}

void rtc_init() {
    // TODO: load some saved time
    rtc_write_time_from_timestamp(0);

    _last_millis = millis();
    _buffer_millis = 0;
}

void rtc_update() {
    int now_millis = millis();
    int delta = now_millis - _last_millis;

    // Suppose that timestamp starts from 01/01/2010
    _buffer_millis += delta;
    
    // How many seconds elapsed?
    int seconds   = _buffer_millis / 1000UL;
    // Keep only the milliseconds that not count to a round number
    _buffer_millis = _buffer_millis % 1000UL;
    
    rtc_now.time.second += seconds;

    state.new_minute = false;

    // Cascade update all time components.
    if (rtc_now.time.second >= 60) {
        rtc_now.time.second = 0;
        rtc_now.time.minute++;

        state.new_minute = true;

        if (rtc_now.time.minute >= 60) {
            rtc_now.time.minute = 0;
            rtc_now.time.hour++;

            if (rtc_now.time.hour >= 24) {
                rtc_now.time.hour = 0;
                rtc_now.date.day++;

                rtc_now.date.dow = ++rtc_now.date.dow % 7;

                if (rtc_now.date.day > _dpm[rtc_now.date.month - 1] + (rtc_now.date.leap && rtc_now.date.month == 2)) {
                    rtc_now.date.day = 1;
                    rtc_now.date.month++;

                    if (rtc_now.date.month > 12) {
                        rtc_now.date.month = 1;
                        rtc_now.date.year++;
                    }
                }
            }
        }
    }

    _last_millis = now_millis;
}

#endif
/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/ 

#include "rtc.h"

#ifdef USE_SW_RTC

DateTime now;

const short days_per_month[]  = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// Sakamoto's costants for DOW calculation
const short dow_constants[]   = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

long last_millis, buffer_millis;

bool is_leap(int year) { 
    return (now.date.year % 4)   == 0 &&
           (now.date.year % 100) != 0 ||
           (now.date.year % 400) == 0;
}

void write_time(DateTime t) { now = t; }

void write_time_from_timestamp(long timestamp) {
    // Get only today's seconds
    int since_midnight = timestamp % 86400UL;

    // Calculate time info.
    now.time.hour   =  since_midnight / 3600;
    now.time.minute = (since_midnight % 3600) / 60;
    now.time.second =  since_midnight % 60;

    // Get how many days passed since 1 Jen 2010.
    int days = timestamp / 86400UL;

    // As said, timestamp = 0 => 1 Jen 2010
    now.date.year = 2010;

    // Starting from 2010, remove every year's days from the count, to get correct year.
    while (days >= 365 + is_leap(now.date.year)) {
        days -= 365 + is_leap(now.date.year);

        now.date.year++;
    }

    // Is the year we just found leap?
    now.date.leap = is_leap(now.date.year);

    // Start from Jen, in base 0
    now.date.month = 0;

    // As for the year, remove month's day from the count, to get correct day.
    while (days >= days_per_month[now.date.month]) {
        days -= days_per_month[now.date.month] + (now.date.leap && (now.date.month == 1));

        now.date.month++;
    }

    // Bring back the month to base 1
    now.date.month++;

    // Do the same for days
    now.date.day = days + 1;

    // Sakamoto algorithm for Day Of Week
    int y = now.date.year - (now.date.month < 3);
    now.date.dow = (y + y/4 - y/100 + y/400 + dow_constants[now.date.month-1] + now.date.day) % 7;
}

void init_rtc() {
    long timestamp = 0;//getTimestamp();

    write_time_from_timestamp(timestamp + TIMEZONE * 60 * 60);

    last_millis = millis();
    buffer_millis = 0;
}

void update_rtc() {
    int now_millis = millis();
    int delta = now_millis - last_millis;

    // Suppose that timestamp starts from 01/01/2010
    buffer_millis += delta;
    
    // How many seconds elapsed?
    int seconds   = buffer_millis / 1000UL;
    // Keep only the milliseconds that not count to a round number
    buffer_millis = buffer_millis % 1000UL;
    
    now.time.second += seconds;

    state.new_minute = false;

    // Cascade update all time components.
    if (now.time.second >= 60) {
        now.time.second = 0;
        now.time.minute++;

        state.new_minute = true;

        if (now.time.minute >= 60) {
            now.time.minute = 0;
            now.time.hour++;

            if (now.time.hour >= 24) {
                now.time.hour = 0;
                now.date.day++;

                now.date.dow = ++now.date.dow % 7;

                if (now.date.day > days_per_month[now.date.month - 1] + (now.date.leap && now.date.month == 2)) {
                    now.date.day = 1;
                    now.date.month++;

                    if (now.date.month > 12) {
                        now.date.month = 1;
                        now.date.year++;
                    }
                }
            }
        }
    }

    last_millis = now_millis;
}

#endif
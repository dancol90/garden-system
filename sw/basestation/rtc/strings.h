/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __strings_h__
#define __strings_h__

const char lun[] = "Lun";
const char mar[] = "Mar";
const char mer[] = "Mer";
const char gio[] = "Gio";
const char ven[] = "Ven";
const char sab[] = "Sab";
const char dom[] = "Dom";

const char* rtc_str_days[] = { dom, lun, mar, mer, gio, ven, sab };

const char mon1[]  = "Gennaio";
const char mon2[]  = "Febbraio";
const char mon3[]  = "Marzo";
const char mon4[]  = "Aprile";
const char mon5[]  = "Maggio";
const char mon6[]  = "Giugno";
const char mon7[]  = "Luglio";
const char mon8[]  = "Agosto";
const char mon9[]  = "Settembre";
const char mon10[] = "Ottobre";
const char mon11[] = "Novembre";
const char mon12[] = "Dicembre";

const char* rtc_str_months[] = { mon1, mon2, mon3, mon4, mon5, mon6, mon7, mon8, mon9, mon10, mon11, mon12 };

#endif
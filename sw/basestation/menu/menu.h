/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __menu_h__
#define __menu_h__

#include <Arduino.h>
#include <MenuLib.h>

#include "../config/config.h"
#include "../display/display.h"
#include "../input/input.h"
#include "../rf/rf.h"
#include "../storage/storage.h"

#ifdef USE_WIFI
#include "../wifi/menu.h"
#endif

#include "../receivers.h"
#include "../settings.h"

extern char job_name[];

void menu_init();
void menu_update();
void menu_enter();

extern MenuController menu;

#endif
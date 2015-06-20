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

#include "../receivers.h"
#include "../settings.h"

#include <LcdDrawer.h>

extern char job_name[];

void init_menu();
void update_menu();
void enter_menu();

extern MenuController menu;

#endif
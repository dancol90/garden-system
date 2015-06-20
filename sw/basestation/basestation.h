/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __basestation_h__
#define __basestation_h__

#include "config/config.h"

#include "storage/storage.h"
#include "input/input.h"
#include "display/display.h"
#include "menu/menu.h"
#include "rtc/rtc.h"
#include "rf/rf.h"
#include "homescreen.h"
#include "receivers.h"

#include "settings.h"

#define selected_receiver receivers[state.selected_recv]

#endif
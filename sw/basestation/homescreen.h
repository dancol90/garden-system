/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/ 

#ifndef __homescreen_h__
#define __homescreen_h__

#include <Arduino.h>
#include "basestation.h"
 
void draw_selector(int x, int y);
void update_homescreen();

#endif
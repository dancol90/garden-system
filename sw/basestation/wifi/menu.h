/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __wifi_menu_h
#define __wifi_menu_h

#include <MenuLib.h>

extern MenuController menu;

void wifi_menu_init(Menu* parent);

void wifi_selected(int i);
void wifi_enter(Menu* m);
void wifi_confirmed(const char* passphrase);

#endif
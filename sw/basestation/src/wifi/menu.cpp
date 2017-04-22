/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "wifi.h"
#include "menu.h"

#include "../menu/menu_custom.h"

#ifdef USE_WIFI

uint8_t   _wifi_index;
MenuItem* _password_input;

void wifi_menu_init(Menu* parent) {
	Menu* wifi_menu = new Menu(parent, F("WiFi"), wifi_enter);
    parent->addItem(wifi_menu);
	_password_input = new PasswordInput(parent, F("Passphrase"), wifi_confirmed);
}

void wifi_selected(int i) {
    _wifi_index = i;

    menu.takeControl(_password_input);
}

void wifi_enter(Menu* m) {
    display_show_message(F("Ricerca..."));

    m->clearItems();

    int n = WiFi.scanNetworks();

    for (int i = 0; i < n; i++) {
        m->addItem(new ParamAction<int>(m, WiFi.SSID(i).c_str(), wifi_selected, i));
    }
}

void wifi_confirmed(const char* passphrase) {
    Serial.print("Connecting to ");
    Serial.println(WiFi.SSID(_wifi_index));
    Serial.print("Password: ");
    Serial.println(passphrase);

	WiFi.begin(
        WiFi.SSID(_wifi_index).c_str(),
        passphrase,
        0,
        WiFi.BSSID(_wifi_index)
    );
}

#endif

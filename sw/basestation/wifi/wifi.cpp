/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/
#include "wifi.h"
#include "menu.h"

#include "../menu/menu_custom.h"

Ticker _rssi_update;
byte _signal_quality;

void _wifi_update_rssi() {
    _signal_quality = 0xFF;

    if(WiFi.status() == WL_CONNECTED) {
        // RSSI is a negative number in dBm
        // rssi <= 100 -> 0% signal
        // rssi >= 50 -> 100% signal
        // else -> 2 * (rssi + 100) percent signal
        int rssi = WiFi.RSSI() + 100;
        _signal_quality = rssi <= 0 ? 0 : (rssi <= 50 ? 2*rssi : 100);
    }

    //WiFi.printDiag(Serial);
}

void wifi_init() {
    _wifi_update_rssi();

    _rssi_update.attach(1, _wifi_update_rssi);

    // async wifi scan to let autoconnect find the right BSSID
    //int n = WiFi.scanNetworks();
}

byte wifi_get_quality() {
	return _signal_quality;
}

// ############################################################################################
uint8_t wifi_index;
MenuItem* password_input;

void wifi_menu_init(Menu* parent) {
	Menu* wifi_menu = new Menu(parent, F("WiFi"), wifi_enter);
    parent->addItem(wifi_menu);
	password_input = new PasswordInput(parent, F("Passphrase"), wifi_confirmed);
}

void wifi_selected(int i) {
    wifi_index = i;

    menu.takeControl(password_input);
}

void wifi_enter(Menu* m) {
    show_message(F("Ricerca..."));

    m->clearItems();

    int n = WiFi.scanNetworks();

    for (int i = 0; i < n; i++) {
        m->addItem(new ParamAction<int>(m, WiFi.SSID(i), wifi_selected, i));
    }
}

void wifi_confirmed(const char* passphrase) {
    Serial.print("Connecting to ");
    Serial.println(WiFi.SSID(wifi_index));
    Serial.print("Password: ");
    Serial.println(passphrase);

    //WiFi.disconnect();
    WiFi.begin(
        WiFi.SSID(wifi_index),
        "phobicviolin310",
        0,
        WiFi.BSSID(wifi_index)
    );
}

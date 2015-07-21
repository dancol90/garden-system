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

// Endless timer to update signal strenght
Ticker _rssi_update;
bool _startup_done;
byte _signal_quality;

// Update the signal strength
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
}


void wifi_init() {
    _wifi_update_rssi();

    // Update signal strength every second
    _rssi_update.attach(1, _wifi_update_rssi);

    _startup_done = false;
}

// Check for connection and init some services
void wifi_update() {
    if(!_startup_done && WiFi.status() == WL_CONNECTED) {
        // NTP time set
        long ts = ntp_get_timestamp();

        write_time_from_timestamp(ts);

        // Job's done, do not repeat again.
        _startup_done = true;
    }
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

#endif
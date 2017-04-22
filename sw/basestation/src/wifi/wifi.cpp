/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include <Ticker.h>
#include "wifi.h"

#ifdef USE_WIFI

// Endless timer to update signal strenght
Ticker _rssi_update;
bool   _startup_done;
byte   _signal_quality;

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

        rtc_write_time_from_timestamp(ts);

        // Job's done, do not repeat again.
        _startup_done = true;
    }
}

byte wifi_get_quality() {
	return _signal_quality;
}

#endif
/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#ifndef __rf_h
#define __rf_h

#include <Arduino.h>

#include "../config/config.h"
#include "../settings.h"
#include "../receivers.h"

#include <SPI.h>
#include <RF24.h>

struct RFPacket {
    uint8_t command;
    uint8_t id;
    uint8_t state;
};

/*class MyRF24 : public RF24 {
    public:
          MyRF24(uint8_t _cepin, uint8_t _cspin) : RF24(_cepin, _cspin) {}

          uint8_t flush_rx(void) { RF24::flush_rx(); };
          uint8_t flush_tx(void) { RF24::flush_tx(); };
};*/

const uint8_t RF24_PAYLOAD = sizeof(RFPacket);

void rf_init();
void rf_update();
void rf_write_tx_fifo();

#endif
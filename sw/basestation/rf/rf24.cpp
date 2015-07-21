/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

#include "rf.h"

MyRF24 _radio(RF24_CE, RF24_CSN);

/*
    ATTENTION!! This definition is just temporary.
    When everything will work, it will be moved to eeprom.ino 
    and it will load the saved ID from the memory.
    There will be also a menu in Settings to select the ID for each channel.

    By now just link pipes with index number, avoiding id=0
*/
#define get_recv_id(i) (i + 1)

void rf_init() {
    _radio.begin();

    // Increase the delay between retries & # of retries
    _radio.setRetries(15,15);

    // Set payload size
    _radio.setPayloadSize(8);

    // High trasmit power
    _radio.setPALevel(RF24_PA_HIGH);
    // Low speed to increase signal range
    _radio.setDataRate(RF24_250KBPS);

    _radio.enableAckPayload();

    // Open a pipe
    for (byte i = 0; i < RECEIVER_COUNT; i++) {
        _radio.openReadingPipe(i, RF24_BASE_PIPE | get_recv_id(i));
    }

    _radio.startListening();

    rf_write_tx_fifo();
}

void rf_update() {
    uint8_t recv_index;

    if (_radio.available(&recv_index)) {
        _radio.flush_rx();

        rf_write_tx_fifo();

        if (recv_index < RECEIVER_COUNT)
            receivers[recv_index].got_rf = true;
    }
}

void rf_write_tx_fifo() {
    RFPacket packet;

    _radio.flush_tx();

    for (byte i = 0; i < RECEIVER_COUNT; i++) {
        // Response packet
        packet.command = 0xF0;

        packet.id = get_recv_id(i);

        // Send the state of the requesting receiver
        packet.state = receivers[i].active;

        _radio.writeAckPayload(i, &packet, sizeof(RFPacket));
    }
}

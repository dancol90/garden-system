/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

class MyRF24 : public RF24 {
    public:
          MyRF24(uint8_t _cepin, uint8_t _cspin) : RF24(_cepin, _cspin) {}

          uint8_t flush_rx(void) { RF24::flush_rx(); };
          uint8_t flush_tx(void) { RF24::flush_tx(); };
};

/*
    ATTENTION!! This definition is just temporary.
    When everything will work, it will be moved to eeprom.ino 
    and it will load the saved ID from the memory.
    There will be also a menu in Settings to select the ID for each channel.

    By now just link pipes with index number, avoiding id=0
*/
#define get_recv_id(i) (i + 1)

MyRF24 radio(RF24_CE, RF24_CSN);

void init_rf() {
    radio.begin();

    // Increase the delay between retries & # of retries
    radio.setRetries(15,15);

    // Set payload size
    radio.setPayloadSize(8);

    // High trasmit power
    radio.setPALevel(RF24_PA_HIGH);
    // Low speed to increase signal range
    radio.setDataRate(RF24_250KBPS);

    radio.enableAckPayload();

    // Open a pipe
    for (byte i = 0; i < RECEIVER_COUNT; i++) {
        radio.openReadingPipe(i, RF24_BASE_PIPE | get_recv_id(i));
    }

    radio.startListening();

    write_tx_fifo();
}

void update_rf() {
    uint8_t recv_index;

    if (radio.available(&recv_index)) {
        radio.flush_rx();

        write_tx_fifo();

        if (recv_index < RECEIVER_COUNT)
            receivers[recv_index].got_rf = true;
    }
}

void write_tx_fifo() {
    Packet packet;

    radio.flush_tx();

    for (byte i = 0; i < RECEIVER_COUNT; i++) {
        // Response packet
        packet.command = 0xF0;

        packet.id = get_recv_id(i);

        // Send the state of the requesting receiver
        packet.state = receivers[i].active;

        radio.writeAckPayload(i, &packet, sizeof(Packet));
    }
}

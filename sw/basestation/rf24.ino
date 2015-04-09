/*############################################################################################
 Garden System
 Wireless controller for watering electropumps

 Author: Daniele Colanardi
 License: BSD, see LICENSE file
############################################################################################*/

/*
    ATTENTION!! This definition is just temporary.
    When everything will work, it will be moved to eeprom.ino 
    and it will load the saved ID from the memory.
    There will be also a menu in Settings to select the ID for each channel.

    By now just link pipes with index number, avoiding id=0
*/
#define get_recv_id(i) (i + 1)

RF24 radio(RF24_CE, RF24_CSN);
Packet packet;

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

    // Open a pipe
    for (byte i = 0; i < RECEIVER_COUNT; i++) {
        radio.openReadingPipe(i, RF24_BASE_PIPE | get_recv_id(i));
    }

    radio.startListening();
}

void update_rf() {
    uint8_t recv_index;

    if (radio.available(&recv_index)) {
        // Fetch the payload
        radio.read(&packet, RF24_PAYLOAD);

        radio.stopListening();

        // Check if it's an expected pipe
        // This should not be necessary, but it's an extra care.
        if (recv_index >= RECEIVER_COUNT) 
            return;

        // Validate the packet
        if (get_recv_id(recv_index) != packet.id)
            return;

        // Response packet
        packet.command = 0xF0;
        // The receiver ID remains the same.
        // Send the state of the requesting receiver
        packet.state = receivers[recv_index].active;

        radio.openWritingPipe(RF24_BASE_PIPE | packet.id);
        radio.write(&packet, sizeof(Packet));

        radio.startListening();
    }
}

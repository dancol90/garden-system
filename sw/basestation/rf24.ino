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

    // Open a writing pipe
    radio.openWritingPipe(0xF0F0F0F0E1LL);
    // Open a reading pipe
    //radio.openReadingPipe(1, in_pipe);

    radio.startListening();
}

void update_rf() {
    
}
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define RF24_CE      10
#define RF24_CSN     A0

RF24 radio(RF24_CE, RF24_CSN);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t out_pipe = 0xF0F0F0F0E1LL;
const uint64_t in_pipe  = 0xF0F0F0F0D2LL;

byte packet[8];
int count = 0;

void setup(void) {
  pinMode(13, OUTPUT);
  
  Serial.begin(115200);
  
  while(!Serial);
  
  Serial.print("Setup -> ");

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
  
  Serial.println("Listening");
  radio.startListening();
}

void loop(void) {
  if (radio.available()) {
    Serial.print("Reading ->");
    
    bool done = false;
    
    while (!done) {
      // Fetch the payload, and see if this was the last one.
      done = radio.read(packet, 8);
    }
    
    // Delay just a little bit to let the other unit
    // make the transition to receiver
    delay(50);
    
    Serial.print("Got ");
      
    for(int i = 0; i < 8; i++) {
      Serial.print(packet[i], HEX); Serial.print(" ");
    }
    
    // First, stop listening so we can talk
    radio.stopListening();
    
    Serial.print(" -> Sending "); Serial.print(count); Serial.print(" -> ");
    
    packet[2] = (count >> 4) & 0xFF;
    packet[3] =  count       & 0xFF;
    
    count++;

    // Send the final one back.
    bool ret = radio.write(packet, 8);
    
    Serial.print(ret);Serial.print(" -> ");
    
    Serial.println("Listening");
    
    // Now, resume listening so we catch the next packets.
    radio.startListening();
  }
}

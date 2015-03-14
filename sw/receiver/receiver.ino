#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// -----------------------------------------

#define MOTOR_1      4
#define MOTOR_2      5

#define RF24_CE      10
#define RF24_CSN     A0

#define RF24_PAYLOAD 8
#define RF24_PIPE    0xF0F0F0F0E1LL

// -----------------------------------------

void goToSleep(uint8_t timeout = 0xFF);

// -----------------------------------------

RF24 radio(RF24_CE, RF24_CSN);

const uint64_t in_pipe  = 0xF0F0F0F0E1LL;
const uint64_t out_pipe = 0xF0F0F0F0D2LL;

const byte request[2] = {0x00, 0xFA};
byte packet[8];

bool timeout, rx, tx, fail;

void setup(void) {
  //Serial.begin(115200);

  // Turn down unneeded peripherals
  ADCSRA = 0;
  power_adc_disable();
  //power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  power_usart0_disable();
  
  // Set sleep tothe most power-saving mode.
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  // Setup and configure rf radio
  radio.begin();

  // Increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // Set payload size
  radio.setPayloadSize(RF24_PAYLOAD);

  // High trasmit power
  radio.setPALevel(RF24_PA_HIGH);
  // Low speed to increase signal range
  radio.setDataRate(RF24_250KBPS);
 
  // Open the communication pipe
  // NOTE: this command will also open a reading pipe at index 0 for auto ack
  //       but is usable to receive data too.
  radio.openWritingPipe(RF24_PIPE);
  
  // Power down nrf24
  radio.powerDown();
  
  // Attach interrupt
  attachInterrupt(1, checkRadio, FALLING);
  
  // Init motor pins
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
}

void loop(void) { 
  // Replace this with Arduino sleep + WDT timeout
  goToSleep(WDTO_8S);

  radio.powerUp();
  // Wait until radio is fully awake
  delay(50);

  // Reset flags
  tx = false; fail = false;

  // Send status request to base. This command power up the radio
  radio.startWrite(request, 2);
  
  goToSleep();

  if(tx) { 
    // Transmission was successful, now wait a reply

    // Put nrf24 in receive mode
    radio.startListening();

    // Reset flags
    rx = false; fail = false;

    // Wait for nrf24 to do its magic
    goToSleep(WDTO_60MS);

    if(rx) {
      // We got something

      // Read it
      radio.read(packet, RF24_PAYLOAD);

      /*Serial.print("-> Got ");

      for(int i = 0; i < RF24_PAYLOAD; i++) {
        Serial.print(packet[i], HEX); Serial.print(" ");
      }
      Serial.println();*/

      // TODO: do something here
      delay(50);
    } /* else {
      // RX timed out. Restart from scratch.
      Serial.println(" -> Rx timeout");  
    } */

    radio.stopListening();

  } /* else {
    Serial.println(" -> Tx Error");
  }*/
  
  // It's done. Now, rest.
  radio.powerDown();
}

void checkRadio(void) {
  // Disable wdt.
  wdt_disable();

  // What happened?
  radio.whatHappened(tx,fail,rx);

  if(tx || fail)
    radio.powerDown();
}


void goToSleep(uint8_t timeout) {
  if(timeout != 0xFF) {
    // Enable register changes
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    // Enable Interrupt mode, 1s timeout
    WDTCSR = (1<<WDIE) | timeout; 
  }

  // Go to sleep
  sleep_mode();
}

ISR(WDT_vect) {
  cli();
 
  // Disable wdt.
  wdt_disable();
  
  sei();
}
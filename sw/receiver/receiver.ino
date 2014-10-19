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

RF24 radio(RF24_CE, RF24_CSN);

const uint64_t in_pipe  = 0xF0F0F0F0E1LL;
const uint64_t out_pipe = 0xF0F0F0F0D2LL;

const byte request[2] = {0x00, 0xFA};
byte packet[8];

bool timeout;

void setup(void) {
  Serial.begin(115200);
  
  Serial.print("Setup -> "); delay(10);
  
  // Turn down unneeded peripherals
  power_adc_disable();
  //power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  //power_usart0_disable();
  
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
  goToSleep(false); Serial.println();
  
  // Here is passed around a second.
  Serial.print("Wake -> ");
  
  radio.powerUp();
  
  // Send status request to base. This command power up the radio
  Serial.print("Request ");
  int res = radio.write(request, 2);
  
  Serial.print(res); Serial.print(" -> ");
  
  // Put nrf24 in receive mode
  radio.startListening();
  
  // Sleep the MCU while waiting for response
  goToSleep(true);
  
  Serial.print(" -> Wake -> Got ");
  
  if(timeout) {
    Serial.print("nothing");

    radio.powerDown();
    
  } else {
    // checkRadio() has been executed, do stuff.    
    for(int i = 0; i < RF24_PAYLOAD; i++) {
      Serial.print(packet[i], HEX); Serial.print(" ");
    }
  }
  
  radio.stopListening();
  
  Serial.print(" -> ");
}


void goToSleep(bool isTimeout) {
  Serial.print("Sleep"); delay(10);
  
  // Enable interupt timeout
  setWdt(isTimeout);
  
  // Go to sleep
  sleep_mode();
}

void checkRadio(void) {
  wdt_disable();
  
  // What happened?
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);
  
  // Did we receive a message?
  if (rx) {
    // Power down nrf24
    radio.powerDown();
    
    // Read the last packet
    radio.read(packet, RF24_PAYLOAD);
  }
}

void setWdt(bool isTimeout) {
  // Disable global interrupts
  cli();
  
  byte bits;
  
  if(isTimeout) {
    bits = (1<<WDP2) /*| (1<<WDP1)*/;
    timeout = false;
  } else {
    bits = (1<<WDP3) | (1<<WDP0);
  }
  
  // Enable register changes
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // Enable Interrupt mode, 1s timeout
  WDTCSR = (1<<WDIE) | bits; 

  
  // Enable global interrupts
  sei();
}

ISR(WDT_vect) {
  cli();
 
  // Disable wdt.
  wdt_disable();
  
  timeout = true;
  
  sei();
}

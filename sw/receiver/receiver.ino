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

#define RECEIVER_ID  0x01

#define RF24_PAYLOAD sizeof(Packet)
#define RF24_PIPE    0xF0F0F0F000LL | RECEIVER_ID


// -----------------------------------------
struct Packet {
    uint8_t command;
    uint8_t id;
    uint8_t state;
} packet;
// -----------------------------------------

void goToSleep(uint8_t timeout = 0xFF);

// -----------------------------------------

RF24 radio(RF24_CE, RF24_CSN);

bool timeout, rx, tx, fail;

void setup(void) {
  // Serial.begin(9600);

  // Turn down unneeded peripherals
  ADCSRA = 0;
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

  radio.enableAckPayload();
  
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

  // Populate packet
  packet.command = 0xF5;
  packet.id = RECEIVER_ID;
  packet.state = true;

  // Reset flags
  tx = false; fail = false;
  // Send status request to base. This command power up the radio
  radio.startWrite(&packet, RF24_PAYLOAD);
  
  // Sleeps the AVR while the radio does its things
  goToSleep();

  if(tx && radio.isAckPayloadAvailable()) { 
    // Transmission was successful, we have a reply in ack payload
      
    // Read it
    radio.read(&packet, RF24_PAYLOAD);

    /*Serial.println("--> Got ");

    for(int i = 0; i < RF24_PAYLOAD; i++) {
      Serial.println(packet.command, HEX);
      Serial.println(packet.id, HEX);
      Serial.println(packet.state, HEX);
    }*/

    digitalWrite(MOTOR_1, LOW);
    digitalWrite(MOTOR_2, HIGH);
    delay(100);
    digitalWrite(MOTOR_2, LOW);

    // TODO: do something here
    //delay(50);
  } else {
    digitalWrite(MOTOR_1, HIGH);
    digitalWrite(MOTOR_2, LOW);
    delay(100);
    digitalWrite(MOTOR_1, LOW);

  }
  
  // It's done. Now, rest.
  radio.powerDown();
}

void checkRadio(void) {
  // Disable wdt.
  wdt_disable();

  // What happened?
  radio.whatHappened(tx, fail, rx);

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
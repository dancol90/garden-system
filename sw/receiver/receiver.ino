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

RF24 radio(RF24_CE, RF24_CSN);
bool current_state, rx_ack, tx, fail;

// -----------------------------------------

void goToSleep(uint8_t timeout = 0xFF);

// -----------------------------------------

void setup(void) {
  // Turn down unneeded peripherals
  ADCSRA = 0;
  power_adc_disable();
  // Can't turn off Timer0 because RF24 lib uses delay()
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

  radio.enableAckPayload();
  
  // Power down nrf24
  radio.powerDown();
  
  // Attach interrupt
  attachInterrupt(1, checkRadio, FALLING);
  
  // Init motor pins
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);

  // Inital state is OFF
  current_state = false;
  setSolenoidState(false);
}

void loop(void) {
  // Radio must be awake to send data.
  radio.powerUp();

  // Populate request packet with current information.
  packet.command = 0xF5;
  packet.id      = RECEIVER_ID;
  packet.state   = true;

  // Reset flags
  rx_ack = false;
  // Send status request to base. This command power up the radio
  radio.startWrite(&packet, RF24_PAYLOAD);
  
  // Sleeps the AVR while the radio does its things
  goToSleep();

  // If base and receiver talked as they should, there must be a packet ready to be read.
  // Don't care if write() wasn't successful or basestation didn't responded, just check if there's some data.
  if(rx_ack) { 
    // Transmission was successful, we have a reply in ack payload
      
    // Read it
    radio.read(&packet, RF24_PAYLOAD);

    // TODO: validate packet
    
    // Debug LED flashing
    setSolenoidState(true);  goToSleep(WDTO_60MS);
    setSolenoidState(false); goToSleep(WDTO_120MS);

    // If basestation says that the state changed, then change it.
    if (packet.state != current_state) {
      // Change the state only the first time
      current_state = packet.state;

      // Actuate the state change
      setSolenoidState(packet.state);
    }
  }
  
  // It's done. Now, rest.
  radio.powerDown();

  // Make the mcu sleep for about 8 seconds.
  goToSleep(WDTO_8S);
}

// -----------------------------------------

/**
  
*/
void goToSleep(uint8_t timeout) {
  // If a timeout is specified, set the watchdog accordingly
  if (timeout != 0xFF) {
    wdt_enable(timeout);
    // Reset the watchdog.
    wdt_reset();
    // Enable the wdt interrupt to wake up the mcu
    WDTCSR |= _BV(WDIE);
  }
  
  // Actually sleep
  sleep_mode();

  // The mcu is awake, disable the wdt (also in case of no timeout)
  wdt_disable();
  // Disable the wdt interrupt, just to be sure.
  WDTCSR &= ~_BV(WDIE);
}

/**
*/
void setSolenoidState(bool open) {
  digitalWrite(MOTOR_1, !open); // LOW if open
  digitalWrite(MOTOR_2,  open); // HIGH if open

  goToSleep(WDTO_120MS);

  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
}

// -----------------------------------------

/**
  Interrupt handler for radio events.
  This is called everytime radio signal an event through IRQ pin.
*/
void checkRadio(void) {
  // Disable current timer.
  wdt_disable();

  // Reset some flag.
  tx = false;
  fail = false;

  // Check what happened in radio operations
  radio.whatHappened(tx, fail, rx_ack);

  // If the operations succeded, then shut the radio down.
  if(tx || fail)
    radio.powerDown();
}

/**
  Interrupt handler for WDT events.
  These are the first instructions that the mcu executes after waking up.
*/
SIGNAL(WDT_vect) {
  wdt_disable();
  wdt_reset();
  WDTCSR &= ~_BV(WDIE);
}

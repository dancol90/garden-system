#include <SPI.h>
#include <Wire.h>

#include <LightPCD8544.h>
#include "nRF24L01.h"
#include "RF24.h"

#include <MenuLib.h>
#include <NokiaLcdDrawer.h>

#include "structures.h"
#include "config.h"
#include "digit_font.h"


#define TRUE  1
#define FALSE 0

// ############################################################################################

Time now;

LightPCD8544 lcd = LightPCD8544(LCD_DC, LCD_CS);

Menu* root = new Menu(NULL, NULL);
Menu* timer_edit;

NokiaLcdDrawer dr = NokiaLcdDrawer(lcd);
MenuController menu = MenuController(root, &dr);

uint8_t backlight = 5, contrast = 5;

// ############################################################################################

long read_vcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

// ############################################################################################

void timer_list_entered(Menu* menu) {
    return;
}

void timer_selected(int index) {
    menu.takeControl(timer_edit);
}

// ############################################################################################
void draw_selector(int x, int y) {
    lcd.drawHLine(x    , y    , 5, BLACK);
    lcd.drawHLine(x + 1, y + 1, 3, BLACK);
    lcd.drawHLine(x + 2, y + 2, 1, BLACK);
}

void draw_homescreen() {
    
    lcd.clearDisplay();

    read_time();

    draw_big_digit(4,          1, now.s.h   / 10);   
    draw_big_digit(6 + font_w, 1, now.s.h   % 10); 

    draw_big_digit(6 + 2*font_w, 1, '.');

    draw_big_digit(14 + 2*font_w, 1, now.s.m / 10);   
    draw_big_digit(16 + 3*font_w, 1, now.s.m % 10);

    lcd.setCursor(1, 26);
    lcd.print("Next: 15:00 \x1a 16:00");

    lcd.fillRect( 0, 39, 42, 9, BLACK);
    lcd.drawChar(18, 40, '1', WHITE);

    lcd.drawRect(42, 39, 42, 9, BLACK);
    lcd.drawChar(60, 40, '2', BLACK);


    draw_selector(18, 35);

    lcd.display();
}

// ############################################################################################

void setup() {
    Serial.begin(9600);
    
    // Setup LCD backlight pin
    pinMode(LCD_LED, OUTPUT);

    init_buttons();

    Wire.begin();

    lcd.begin();
    lcd.setContrast(50);
    // Turn on backlight
    analogWrite(LCD_LED, 200);

    
    Menu* sub;

    root->setText(F("Menu"));

    root->addItem(new Action(root, F("Forza accensione")));
    root->addItem(new Action(root, F("Forza spegnimento")));

    sub = new Menu(root, F("Timer"), &timer_list_entered);

        sub->addItem(new Action(sub, F("Aggiungi"), timer_selected, 1));
        sub->addItem(new Action(sub, F("15:00 -> 16:10"), timer_selected, 2));

        root->addItem(sub);


    timer_edit = new Menu(sub, F("Edit"));

    sub = new Menu(root, F("Impostazioni"));

        sub->addItem(new NumericSelector(sub, F("Retroilluminazione"), (int&)backlight, 1, 10));
        sub->addItem(new NumericSelector(sub, F("Contrasto"), (int&)contrast, 1, 10));

        root->addItem(sub);


    // timer_edit->addItem(new NumericSelector(timer_edit, F("Ore"), h, 0, 24));
    // timer_edit->addItem(new NumericSelector(timer_edit, F("Minuti"), m, 0, 60));
    // timer_edit->addItem(new Action(timer_edit, F("Elimina")));
}

void loop() {

    draw_homescreen();

    return;

    update_buttons();

    if (is_pressed(0))
        menu.prev();
    else if (is_pressed(1))
        menu.next();
    else if (is_pressed(2))
        menu.select();
    else if (is_pressed(3))
        menu.back();


    menu.draw();
}

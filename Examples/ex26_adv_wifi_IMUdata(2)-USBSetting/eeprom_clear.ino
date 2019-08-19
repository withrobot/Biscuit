#include <EEPROM.h>

#define PIN_LED 12

void setup() {
  
  pinMode(PIN_LED, OUTPUT);
  EEPROM.begin(512);
  
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  
  digitalWrite(PIN_LED,LOW);
}

void loop() {
  
}

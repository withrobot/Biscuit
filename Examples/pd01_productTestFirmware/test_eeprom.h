#pragma once
#include <arduino.h>

void eeprom_init(void);
bool eeprom_test(void);
void eeprom_read(int addr, uint8_t* buf, int len);
void eeprom_write(int addr, uint8_t* buf, int len, bool commit = true);

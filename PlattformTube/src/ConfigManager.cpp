#include "ConfigManager.h"
#include <EEPROM.h>

void ConfigManager::loadFromEEPROM() {
  EEPROM.begin(4);
  dmxAddress = EEPROM.read(0);
  if (dmxAddress <= 0 || dmxAddress > 512) dmxAddress = 1;
}

void ConfigManager::saveToEEPROM() {
  EEPROM.write(0, dmxAddress);
  EEPROM.commit();
}

int ConfigManager::getAddress() {
  return dmxAddress;
}
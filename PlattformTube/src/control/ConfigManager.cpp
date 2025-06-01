#include "control/ConfigManager.h"
#include <EEPROM.h>

/**
 * @brief Loads the DMX address from EEPROM.
 * 
 * Reads the value stored at address 0 in EEPROM and assigns it to dmxAddress.
 * If the value is out of valid DMX range (1–512), it defaults to 1.
 */
void ConfigManager::loadFromEEPROM() {
  EEPROM.begin(4); // Initialize EEPROM with 4 bytes of space
  dmxAddress = EEPROM.read(0);
  if (dmxAddress <= 0 || dmxAddress > 512) dmxAddress = 1;
}

/**
 * @brief Saves the current DMX address to EEPROM.
 * 
 * Writes dmxAddress to EEPROM at address 0 and commits the change.
 */
void ConfigManager::saveToEEPROM() {
  EEPROM.write(0, dmxAddress);
  EEPROM.commit();
}

/**
 * @brief Gets the current DMX address.
 * 
 * @return int The DMX address.
 */
int ConfigManager::getAddress() {
  return dmxAddress;
}

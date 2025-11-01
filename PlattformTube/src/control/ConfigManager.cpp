#include "control/ConfigManager.h"
#include <EEPROM.h>


ConfigManager::ConfigManager(uint16_t artnetUniverse, uint16_t dmxAddr, DMXReceivers inputType, DmxMode mode)
{
  this->dmxAddress = dmxAddr;
  this->artnetUniverse = artnetUniverse;
  this->dmxInputType = static_cast<uint8_t>(inputType);
  this->dmxMode = static_cast<uint8_t>(dmxMode);
  Serial.print("dmxMode :");
  Serial.println(this->dmxMode);
  saveToEEPROM();
}

/**
 * @brief Loads the DMX address from EEPROM.
 *
 * Reads the value stored at address 0 in EEPROM and assigns it to dmxAddress.
 * If the value is out of valid DMX range (1–512), it defaults to 1.
 */
void ConfigManager::loadFromEEPROM()
{
  
  dmxAddress = EEPROM.read(0);
  artnetUniverse = EEPROM.read(1);
  dmxInputType = EEPROM.read(2);
  dmxMode = EEPROM.read(3);
  if (dmxAddress <= 0 || dmxAddress > 512)
    dmxAddress = 1;
  if (artnetUniverse < 0)
    artnetUniverse = 1;
}

/**
 * @brief Saves the current DMX address to EEPROM.
 *
 * Writes dmxAddress to EEPROM at address 0 and commits the change.
 */
void ConfigManager::saveToEEPROM()
{
  EEPROM.write(0, dmxAddress);
  EEPROM.write(1, artnetUniverse);
  EEPROM.write(2, dmxInputType);
  EEPROM.write(3, dmxMode);
  EEPROM.commit();
}

/**
 * @brief Gets the current DMX address.
 *
 * @return int The DMX address.
 */
uint16_t ConfigManager::getDmxAddress()
{
  return dmxAddress;
}

/**
 * @brief Gets the current DMX universe for artnet.
 *
 * @return int The artnet universe.
 */
uint16_t ConfigManager::getArtnetUniverse()
{
  return dmxAddress;
}

/**
 * @brief Gets the current DMX inputtype (wired or artnet)
 *
 * @return Enum DmxInputType
 */
DMXReceivers ConfigManager::getDmxInputType()
{
  return static_cast<DMXReceivers>(dmxInputType);
}

/**
 * @brief Gets the current DMX mode of the lamp (DMX 1, 4, 32 etc.)
 *
 * @return current DmxMode enum value
 */
DmxMode ConfigManager::getDmxMode()
{
  return static_cast<DmxMode>(dmxMode);
}


void ConfigManager::printConfig(){
  Serial.println("########### CONFIG #############");
  Serial.print("dmxAddress: ");
  Serial.println(dmxAddress);
  Serial.print("artnetUniverse: ");
  Serial.println(artnetUniverse);
  Serial.print("dmxInputType: ");
  Serial.println(dmxInputType);
  Serial.print("dmxMode: ");
  Serial.println(dmxMode);
  Serial.println("################################");
}
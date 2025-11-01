#include "control/ConfigManager.h"
#include <EEPROM.h>

ConfigManager::ConfigManager()
{
  mutex = xSemaphoreCreateMutex();
  loadFromEEPROM();
}

ConfigManager::ConfigManager(uint16_t artnetUniverse, uint16_t dmxAddr, DMXReceivers inputType, DmxMode mode)
{
  mutex = xSemaphoreCreateMutex();
  
  this->dmxAddress = dmxAddr;
  this->artnetUniverse = artnetUniverse;
  this->dmxInputType = static_cast<uint8_t>(inputType);
  this->dmxMode = static_cast<uint8_t>(dmxMode);
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
  EEPROM.get(0, dmxAddress);
  EEPROM.get(2, artnetUniverse);
  EEPROM.get(4, dmxInputType);
  EEPROM.get(5, dmxMode);
  if (dmxAddress <= 0 || dmxAddress > 512)
    dmxAddress = 0;
  if (artnetUniverse < 0)
    artnetUniverse = 0;
}

/**
 * @brief Saves the current DMX address to EEPROM.
 *
 * Writes dmxAddress to EEPROM at address 0 and commits the change.
 */
void ConfigManager::saveToEEPROM()
{
  EEPROM.put(0, dmxAddress);
  EEPROM.put(2, artnetUniverse);
  EEPROM.put(4, dmxInputType);
  EEPROM.put(5, dmxMode);
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

void ConfigManager::printConfig()
{
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

uint16_t ConfigManager::setDmxAddress(uint16_t dmxAddress)
{
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->dmxAddress = dmxAddress;
  xSemaphoreGive(mutex);
  return dmxAddress;
}
uint16_t ConfigManager::setArtnetUniverse(uint16_t artnetUniverse)
{
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->artnetUniverse  = artnetUniverse;
  xSemaphoreGive(mutex);
  return artnetUniverse;
}
DmxMode ConfigManager::setDmxMode(DmxMode dmxMode)
{
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->dmxMode = dmxMode;
  xSemaphoreGive(mutex);
  return dmxMode;
}
DMXReceivers ConfigManager::setDmxInputType(DMXReceivers receiverType){
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->dmxInputType = receiverType;
  xSemaphoreGive(mutex);
  return receiverType;
}
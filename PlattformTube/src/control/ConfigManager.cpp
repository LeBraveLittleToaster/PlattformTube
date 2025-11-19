#include "control/ConfigManager.h"
#include <EEPROM.h>

ConfigManager::ConfigManager()
{
  mutex = xSemaphoreCreateMutex();
  Serial.println("ConfigManager: Loading config from EEPROM");
  loadFromEEPROM();
  Serial.println("ConfigManager: Loaded config:");
  printConfig();
}

ConfigManager::ConfigManager(uint16_t artnetUniverse, uint16_t dmxAddr, DmxReceiverType inputType, DmxMode mode)
{
  mutex = xSemaphoreCreateMutex();
  
  this->dmxAddress = dmxAddr;
  this->artnetUniverse = artnetUniverse;
  this->dmxReceiverType = static_cast<uint8_t>(inputType);
  this->dmxMode = static_cast<uint8_t>(dmxMode);
  saveToEEPROM();
}

void ConfigManager::registerReceiverUpdateCallback(void (*callback)(DmxReceiverType dmxReceivers)){
  this->receiverUpdate = callback;
}
void ConfigManager::registerDmxModeUpdateCallback(void (*callback)(DmxMode dmxMode)){
  this->dmxModeUpdate = callback;
}

/**
 * @brief Loads the DMX address from EEPROM.
 *
 * Reads the value stored at address 0 in EEPROM and assigns it to dmxAddress.
 * If the value is out of valid DMX range (1–512), it defaults to 1.
 */
void ConfigManager::loadFromEEPROM()
{
  EEPROM.get(dmxAddressEEPROMAddress, this->dmxAddress);
  EEPROM.get(artnetUniverseEEPROMAddress, this->artnetUniverse);
  EEPROM.get(dmxReceiverTypeEEPROMAddress, this->dmxReceiverType);
  EEPROM.get(dmxModeEEPROMAddress, this->dmxMode);
  if (this->dmxAddress <= 0 || this->dmxAddress > 512)
    this->dmxAddress = 0;
  if (this->artnetUniverse < 0)
    this->artnetUniverse = 0;
}

/**
 * @brief Saves the current DMX address to EEPROM.
 *
 * Writes dmxAddress to EEPROM at address 0 and commits the change.
 */
void ConfigManager::saveToEEPROM()
{
  EEPROM.put(dmxAddressEEPROMAddress, this->dmxAddress);
  EEPROM.put(artnetUniverseEEPROMAddress, this->artnetUniverse);
  EEPROM.put(dmxReceiverTypeEEPROMAddress, this->dmxReceiverType);
  EEPROM.put(dmxModeEEPROMAddress, this->dmxMode);
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
DmxReceiverType ConfigManager::getDmxReceiverType()
{
  return static_cast<DmxReceiverType>(dmxReceiverType);
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
  Serial.print(dmxAddress);
  Serial.println(", available channels: 0-512 (- dmxMode channels)");
  Serial.print("artnetUniverse: ");
  Serial.print(artnetUniverse);
  Serial.println(", available channels: 0-4096");
  Serial.print("dmxReceiverType: ");
  Serial.print(dmxReceiverType);
  Serial.println(", available types: 0=WIRED_DMX, 1=ARTNET");
  Serial.print("dmxMode: ");
  Serial.print(dmxMode);
  Serial.println(", available modes: 0=DMX_1, 1=DMX_5, 2=DMX_30, 3=DMX_40, 4=DMX_80");
  Serial.println("################################");
}

boolean ConfigManager::setDmxAddress(uint16_t dmxAddress)
{
  if(this->dmxAddress == dmxAddress) {
    return dmxAddress;
  }
  Serial.print("Setting DMX Mode to ");
  Serial.println(static_cast<int>(dmxMode));
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->dmxAddress = dmxAddress;
  saveToEEPROM();
  xSemaphoreGive(mutex);
  return true;
}
boolean ConfigManager::setArtnetUniverse(uint16_t artnetUniverse)
{
  if(this->artnetUniverse == artnetUniverse) {
    return dmxAddress;
  }
  Serial.print("Setting DMX Mode to ");
  Serial.println(static_cast<int>(dmxMode));
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->artnetUniverse  = artnetUniverse;
  saveToEEPROM();
  xSemaphoreGive(mutex);
  return true;
}
boolean ConfigManager::setDmxMode(DmxMode dmxMode)
{
  if(this->dmxMode == dmxMode) {
    return false;
  }
  Serial.print("Setting DMX Mode to ");
  Serial.println(static_cast<int>(dmxMode));
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->dmxMode = dmxMode;
  dmxModeUpdate(dmxMode);
  xSemaphoreGive(mutex);
  saveToEEPROM();
  return true;
}
boolean ConfigManager::setDmxReceiverType(DmxReceiverType dmxReceiverType){
  if(this->dmxReceiverType == dmxReceiverType) {
    return false;
  }
  Serial.print("Setting DMX Mode to ");
  Serial.println(static_cast<int>(dmxMode));
  xSemaphoreTake(mutex,portMAX_DELAY);
  this->dmxReceiverType = dmxReceiverType;
  receiverUpdate(dmxReceiverType);
  saveToEEPROM();
  xSemaphoreGive(mutex);
  return true;
}
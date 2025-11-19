#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#pragma once


#include <Arduino.h>
#include "DMXMode.h"
#include "drivers/IDMXReceiver.h"

class ConfigManager {
public:
    ConfigManager();
    ConfigManager(uint16_t artnetUni, uint16_t dmxAddr, DmxReceiverType inputType, DmxMode mode);
    void printConfig();
    void loadFromEEPROM();
    void saveToEEPROM();
    void registerReceiverUpdateCallback(void (*callback)(DmxReceiverType dmxReceivers));
    void registerDmxModeUpdateCallback(void (*callback)(DmxMode dmxMode));
    boolean setDmxAddress(uint16_t dmxAddress);
    boolean setArtnetUniverse(uint16_t artnetUniverse);
    boolean setDmxMode(DmxMode dmxMode);
    boolean setDmxReceiverType(DmxReceiverType receiverType);
    uint16_t getDmxAddress();
    uint16_t getArtnetUniverse();
    DmxMode getDmxMode();
    DmxReceiverType getDmxReceiverType();
private:
    uint16_t dmxAddress = 1;
    uint16_t artnetUniverse = 1;
    uint8_t dmxReceiverType = 1;
    uint8_t dmxMode = 1;
    uint16_t dmxAddressEEPROMAddress = 0;
    uint16_t artnetUniverseEEPROMAddress = 2;
    uint16_t dmxReceiverTypeEEPROMAddress = 4;
    uint16_t dmxModeEEPROMAddress = 6;
    SemaphoreHandle_t mutex;
    void (*receiverUpdate)(DmxReceiverType dmxReceiverType);
    void (*dmxModeUpdate)(DmxMode dmxMode);
};

#endif // CONFIGMANAGER_H
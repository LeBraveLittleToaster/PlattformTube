
#pragma once

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <Arduino.h>
#include "DMXMode.h"
#include "drivers/IDMXReceiver.h"

class ConfigManager {
public:
    ConfigManager(uint16_t artnetUni, uint16_t dmxAddr, DMXReceivers inputType, DmxMode mode);
    void printConfig();
    void loadFromEEPROM();
    void saveToEEPROM();
    uint16_t getDmxAddress();
    uint16_t getArtnetUniverse();
    DmxMode getDmxMode();
    DMXReceivers getDmxInputType();
private:
    uint16_t dmxAddress = 1;
    uint16_t artnetUniverse = 1;
    uint8_t dmxInputType = 1;
    uint8_t dmxMode = 1;
};

#endif // CONFIGMANAGER_H
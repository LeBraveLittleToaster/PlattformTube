#include <Arduino.h>
#pragma once

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H


class ConfigManager {
public:
    void loadFromEEPROM();
    void saveToEEPROM();
    uint8_t getAddress();
private:
    uint8_t dmxAddress = 1;
};

#endif // CONFIGMANAGER_H
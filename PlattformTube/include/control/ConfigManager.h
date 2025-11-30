#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#pragma once


#include <Arduino.h>
#include "DMXMode.h"
#include "drivers/IDMXReceiver.h"
#include <Preferences.h>

class ConfigManager {
public:
    ConfigManager();
    ConfigManager(uint16_t artnetUni, uint16_t dmxAddr, DmxReceiverType inputType, DmxMode mode, char* wifiSSID, char* wifiPassword);
    void begin(boolean writeToPrefs);
    void printConfig();
    void loadFromEEPROM();
    void saveToEEPROM();
    void registerReceiverUpdateCallback(void (*callback)(DmxReceiverType dmxReceivers));
    void registerDmxModeUpdateCallback(void (*callback)(DmxMode dmxMode));
    boolean setDmxAddress(uint16_t dmxAddress);
    boolean setArtnetUniverse(uint16_t artnetUniverse);
    boolean setDmxMode(DmxMode dmxMode);
    boolean setDmxReceiverType(DmxReceiverType receiverType);
    boolean setWiFiCredentials(char* ssid, char* password);
    char* getWiFiSSID();
    char* getWiFiPassword();
    uint16_t getDmxAddress();
    uint16_t getArtnetUniverse();
    DmxMode getDmxMode();
    DmxReceiverType getDmxReceiverType();
private:
    uint16_t dmxAddress = 1;
    uint16_t artnetUniverse = 1;
    uint8_t dmxReceiverType = 1;
    uint8_t dmxMode = 1;
    String dmxAddrsPref = "DMX_ADDR";
    String artnetUniversePref = "ARTNET_UNI";
    String dmxReceiverTypePref = "DMX_RX_TYPE";
    String dmxModePref = "DMX_MODE";
    String wifiSsidPref = "WIFI_SSID";
    String wifiPasswordPref = "WIFI_PASSWORD";
    char* wifiSSID = "";
    char* wifiPassword = "";
    SemaphoreHandle_t mutex;
    void (*receiverUpdate)(DmxReceiverType dmxReceiverType);
    void (*dmxModeUpdate)(DmxMode dmxMode);
    Preferences preferences;
    
};

#endif // CONFIGMANAGER_H
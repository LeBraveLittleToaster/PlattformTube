#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class ConfigManager {
public:
    void loadFromEEPROM();
    void saveToEEPROM();
    int getAddress();
private:
    int dmxAddress = 1;
};

#endif // CONFIGMANAGER_H
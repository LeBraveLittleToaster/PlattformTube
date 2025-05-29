#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <Arduino.h>
#include "ILEDDriver.h"

class EffectManager {
public:
    void setEffect(int id);
    void applyEffect(ILEDDriver* driver);
private:
    int currentEffect = 0;
};

#endif // EFFECTMANAGER_H
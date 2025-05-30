#ifndef LIGHTTUBE_H
#define LIGHTTUBE_H

#include "IDMXReceiver.h"
#include "drivers/ILEDDriver.h"
#include "ConfigManager.h"

class Lighttube {
public:
    Lighttube(IDMXReceiver* dmx, ILEDDriver* leds);
    void setup();
    void loop();

private:
    IDMXReceiver* dmx;
    ILEDDriver* leds;
    ConfigManager config;
};

#endif // LIGHTTUBE_H
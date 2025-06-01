#include "IDMXReceiver.h"
#include "drivers/ILEDDriver.h"
#include "ConfigManager.h"
#include "control/Ticker.h"
#include "control/Player.h"
#include "Segment.h"

#ifndef LIGHTTUBE_H
#define LIGHTTUBE_H

class LightTube {
public:
    LightTube(IDMXReceiver* dmx, ILEDDriver* leds, Ticker* ticker, uint8_t segmentCount, DmxMode dmxMode);
    void setup();
    void loop();
    void print();
    ~LightTube();

private:
    IDMXReceiver* dmx;
    ConfigManager config;
    Ticker* ticker;
    DMXPlayer* dmxPlayer;
};

#endif // LIGHTTUBE_H
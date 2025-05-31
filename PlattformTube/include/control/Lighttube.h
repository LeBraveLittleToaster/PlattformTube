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
    LightTube(IDMXReceiver* dmx, ILEDDriver* leds, Ticker* ticker, int segmentCount);
    void setup();
    void loop();
    void print();
    ~LightTube();

private:
    IDMXReceiver* dmx;
    ILEDDriver* leds;
    ConfigManager config;
    Ticker* ticker;
    Segment* segments;
    uint8_t segmentCount;
    DMXPlayer dmxPlayer;
};

#endif // LIGHTTUBE_H
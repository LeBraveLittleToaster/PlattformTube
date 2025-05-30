#ifndef LIGHTTUBE_H
#define LIGHTTUBE_H

#include "IDMXReceiver.h"
#include "drivers/ILEDDriver.h"
#include "ConfigManager.h"
#include "control/Ticker.h"
#include "TubeSegment.h"

class Lighttube {
public:
    Lighttube(IDMXReceiver* dmx, ILEDDriver* leds, Ticker* ticker, int segmentCount);
    void setup();
    void loop();
    ~Lighttube();

private:
    IDMXReceiver* dmx;
    ILEDDriver* leds;
    ConfigManager config;
    Ticker* ticker;
    TubeSegment* segments;
    uint8_t segmentCount;
};

#endif // LIGHTTUBE_H
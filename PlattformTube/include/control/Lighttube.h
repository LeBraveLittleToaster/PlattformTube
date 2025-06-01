#include "IDMXReceiver.h"
#include "drivers/ILEDDriver.h"
#include "ConfigManager.h"
#include "control/Ticker.h"
#include "control/Player.h"
#include "Segment.h"

#ifndef LIGHTTUBE_H
#define LIGHTTUBE_H
Segment* getSegments(uint8_t segmentCount, ILEDDriver* driver);
DMXPlayer* getDMXPlayer(DmxMode dmxMode, int segmentCount, ILEDDriver *driver);
class LightTube {
public:
    LightTube(IDMXReceiver* dmx, Ticker* ticker, ConfigManager* config, DMXPlayer* dmxPlayer);
    void setup();
    void loop();
    void print();
    ~LightTube();

private:
    IDMXReceiver* dmx;
    ConfigManager* config;
    Ticker* ticker;
    DMXPlayer* dmxPlayer;
};

#endif // LIGHTTUBE_H;
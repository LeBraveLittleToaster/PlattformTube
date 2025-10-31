#ifndef ARTNET_H
#define ARTNET_H

#include <ArtnetWifi.h>

class Artnet {
public:
    virtual void loop();
    virtual void setup(void (*onDmxFramePtr)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data));

private:
    ArtnetWifi artnet;
    virtual boolean ConnectWifi(void);
};


#endif // ARTNET_H
#ifndef ARTNET_H
#define ARTNET_H

#include <ArtnetWifi.h>
#include "control/ConfigManager.h"
#include "drivers/IDMXReceiver.h"

class Artnet : public IDMXReceiver
{
public:
    Artnet(ConfigManager* config);
    ~Artnet() override;
    virtual void loop() override;
    virtual void begin() override;
    virtual void stop() override;
    virtual uint8_t getChannel(int channel) override;
    int getBufferSize() override;
    virtual bool readData() override;
    virtual uint8_t* getBuffer() override;

private:
    static Artnet* self_;
    ArtnetWifi artnet;
    ConfigManager* config;
    static const int ARTNET_PACKET_SIZE = 512;
    uint8_t dmxBuffer[ARTNET_PACKET_SIZE];
    virtual boolean ConnectWifi(void);
    static void onDmxStatic(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data);
    virtual void onDmx(uint16_t universe, uint16_t length, uint8_t sequence, const uint8_t *data);
};

#endif // ARTNET_H
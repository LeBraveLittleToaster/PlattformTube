#ifndef DMXMAX485_H
#define DMXMAX485_H

#include <esp_dmx.h>

#include "drivers/IDMXReceiver.h"


class DMXMAX485 : public IDMXReceiver {
public:
    DMXMAX485(
        dmx_port_t dmxPort,
        int rxPin,
        int txPin,
        int enPin
    );

    virtual void begin() override;
    virtual bool readData() override;
    virtual void loop() override;
    uint8_t* getBuffer() override;
    int getBufferSize() override;
    uint8_t getChannel(int channel) override;
    virtual ~DMXMAX485();

private:
    dmx_port_t dmxPort;
    int rxPin;
    int txPin;
    int enPin;
    static const int DMX_BUFFER_SIZE = DMX_PACKET_SIZE;
    uint8_t dmxBuffer[DMX_BUFFER_SIZE];
    bool dmxIsConnected = false;
};

#endif // DMXMAX485_H
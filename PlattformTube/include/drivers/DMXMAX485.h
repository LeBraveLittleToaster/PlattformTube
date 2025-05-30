#ifndef DMXMAX485_H
#define DMXMAX485_H

#include "control/IDMXReceiver.h"
#include <HardwareSerial.h>

class DMXMAX485 : public IDMXReceiver {
public:
    DMXMAX485(HardwareSerial& serial, int rxPin);
    virtual void begin() override;
    virtual bool readData() override;
    const uint8_t* getBuffer() const;
    uint8_t getChannel(int channel) override;

    virtual ~DMXMAX485() {}

private:
    HardwareSerial& serial;
    int rxPin;
    static const int DMX_BUFFER_SIZE = 512;
    uint8_t dmxBuffer[DMX_BUFFER_SIZE];
};

#endif // DMXMAX485_H
#ifndef IDMXRECEIVER_H
#define IDMXRECEIVER_H

#include <Arduino.h>

enum DmxReceiverType {
    WIRED_DMX = 0,
    ARTNET = 1
};

class IDMXReceiver {
public:
    
    virtual void begin() = 0;
    virtual void loop() = 0;
    virtual void stop() = 0;
    virtual bool readData() = 0;
    virtual uint16_t* getBuffer() = 0;
    virtual uint16_t getBufferSize() = 0;
    virtual uint16_t getChannel(int channel) = 0;
    virtual ~IDMXReceiver() = default;
};

#endif // IDMXRECEIVER_H
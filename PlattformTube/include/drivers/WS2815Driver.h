#ifndef WS2815DRIVER_H
#define WS2815DRIVER_H

#include "ILEDDriver.h"
#include <Adafruit_NeoPixel.h>

class WS2815Driver : public ILEDDriver {
public:
    WS2815Driver(int dataPin, int numLeds);
    void begin() override;
    void setBrightness(uint8_t brightness);
    void clear() override;
    void show() override;
    void setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w) override;
    void setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w) override;
    void setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w) override;
    uint8_t getTotalPixelCount() override;
    ~WS2815Driver();

private:
    int dataPin;
    int numLeds;
    Adafruit_NeoPixel strip;
};

#endif // WS2815DRIVER_H
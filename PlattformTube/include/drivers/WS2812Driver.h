#ifndef WS2812DRIVER_H
#define WS2812DRIVER_H

#include "ILEDDriver.h"
#include <Adafruit_NeoPixel.h>

class WS2812Driver : public ILEDDriver {
public:
    WS2812Driver(int dataPin, int numLeds);
    void begin() override;
    void setBrightness(uint8_t brightness);
    void clear() override;
    void show() override;
    void setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w) override;
    void setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w) override;
    void setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w) override;
    ~WS2812Driver();

private:
    int dataPin;
    int numLeds;
    Adafruit_NeoPixel* leds;
};

#endif // WS2812DRIVER_H
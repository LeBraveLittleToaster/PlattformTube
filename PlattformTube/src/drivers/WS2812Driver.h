#ifndef WS2812DRIVER_H
#define WS2812DRIVER_H

#include "ILEDDriver.h"
#include <FastLED.h>

class WS2812Driver : public ILEDDriver {
public:
    WS2812Driver(int dataPin, int numLeds);
    void begin() override;
    void setBrightness(uint8_t brightness);
    void clear() override;
    void show() override;
    void setPixel(int idx, uint8_t r, uint8_t g, uint8_t b);

private:
    int dataPin;
    int numLeds;
    CRGB* leds;
};

#endif // WS2812DRIVER_H
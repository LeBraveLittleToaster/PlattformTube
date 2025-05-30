#include "config/HardwareConfig.h"
#include "drivers/WS2812Driver.h"
#include <Adafruit_NeoPixel.h>

WS2812Driver::WS2812Driver(int dataPin, int numLeds)
    : dataPin(dataPin), numLeds(numLeds)
{
    leds = new Adafruit_NeoPixel(numLeds, dataPin, NEO_RGBW + NEO_KHZ800);
}

WS2812Driver::~WS2812Driver()
{
    delete leds;
}

void WS2812Driver::begin()
{
    Serial.println("Begin WS2812b");
    // Serial.println("WS2812 LEDs initialized");
    // Serial.print("Number of LEDs: ");
    // Serial.println(numLeds);
    // Serial.print("LED Pin:");
    // Serial.println(dataPin);
}

void WS2812Driver::setBrightness(uint8_t brightness)
{
    leds->setBrightness(brightness);
}

void WS2812Driver::clear()
{
    for (int i = 0; i < numLeds; ++i)
    {
        leds->setPixelColor(i, 0, 0, 0, 0);
    }
}

void WS2812Driver::show()
{
    leds->show();
    // Serial.println("Showing WS2812 LEDs");
}

void WS2812Driver::setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        leds->setPixelColor(idx, r, g, b, w);
    }
}

void WS2812Driver::setPixelHSV(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        uint32_t rgb = leds->gamma32(leds->ColorHSV(h,s,v));
        leds->setPixelColor(idx, rgb);
    }
}
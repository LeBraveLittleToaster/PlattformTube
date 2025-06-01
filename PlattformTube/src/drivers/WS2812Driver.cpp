#include "config/HardwareConfig.h"
#include "drivers/WS2812Driver.h"
#include <Adafruit_NeoPixel.h>

WS2812Driver::WS2812Driver(int dataPin, int numLeds)
    : dataPin(dataPin), numLeds(numLeds)
{
    
}

WS2812Driver::~WS2812Driver()
{
    delete strip;
}

uint8_t WS2812Driver::getTotalPixelCount()
{
    return numLeds;
}

void WS2812Driver::begin()
{
    Serial.println("WS2812 LEDs initialized");
    Serial.print("Number of LEDs: ");
    Serial.println(numLeds);
    Serial.print("LED Pin:");
    Serial.println(dataPin);
    
    strip = new Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800);
    strip->setBrightness(255);
    strip->clear();
    strip->show();
    
}

uint8_t WS2812Driver::getBrightness()
{
    return strip->getBrightness();
};

uint32_t WS2812Driver::getPixelValue(uint8_t idx)
{
    return strip->getPixelColor(idx);
};

void WS2812Driver::setBrightness(uint8_t brightness)
{
    strip->setBrightness(brightness);
}

void WS2812Driver::clear()
{
    for (int i = 0; i < numLeds; ++i)
    {
        strip->setPixelColor(i, 0, 0, 0, 0);
    }
}

void WS2812Driver::show()
{
    strip->show();
    // Serial.println("Showing WS2812 LEDs");
}

void WS2812Driver::setPixelRGB(int idx, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        strip->setPixelColor(idx, r, g, b, w);
    }
}

void WS2812Driver::setPixelHSV8(int idx, uint8_t h, uint8_t s, uint8_t v, uint8_t w)
{
    uint16_t h16 = (uint16_t)h * 257;
    if (idx >= 0 && idx < numLeds)
    {
        uint32_t rgb = strip->gamma32(strip->ColorHSV(h16, s, v));
        strip->setPixelColor(idx, rgb);
    }
}

void WS2812Driver::setPixelHSV16(int idx, uint16_t h, uint8_t s, uint8_t v, uint8_t w)
{
    if (idx >= 0 && idx < numLeds)
    {
        uint32_t rgb = strip->gamma32(strip->ColorHSV(h, s, v));
        strip->setPixelColor(idx, rgb);
    }
}
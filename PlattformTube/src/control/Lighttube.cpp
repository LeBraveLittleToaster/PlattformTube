#include "control/Lighttube.h"
#include "config/HardwareConfig.h"

// Example dimmer curves
static uint8_t dimmerCurve1(uint8_t value)
{
    // Linear dimmer curve
    return value;
}
static uint8_t dimmerCurve2(uint8_t value)
{
    // S-curve (quadratic)
    return (value * value) / 255;
}

Lighttube::Lighttube(IDMXReceiver *dmx, ILEDDriver *leds)
    : dmx(dmx), leds(leds) {}

void Lighttube::setup()
{
    dmx->begin();
    leds->begin();
    config.loadFromEEPROM();
    leds->setBrightness(255);
    leds->clear();
    leds->show();
}

void Lighttube::loop()
{
    if (dmx->readData())
    {
        const uint8_t *dmxData = dmx->getBuffer();

        // DMX layout: [0]=dimmer, [1..N*3]=pixels, [last]=config
        uint8_t globalDimmer = dmxData[0];
        uint8_t configValue = dmxData[1 + NUM_LEDS * 3];

        // Select dimmer curve
        uint8_t (*dimmerFunc)(uint8_t) = dimmerCurve1;
        if (configValue >= 200)
        {
            dimmerFunc = dimmerCurve2;
        }
        else if (configValue >= 150)
        {
            dimmerFunc = dimmerCurve1;
        }

        for (int i = 0; i < NUM_LEDS; ++i)
        {
            int base = 1 + i * 3;
            uint8_t r = dmxData[base];
            uint8_t g = dmxData[base + 1];
            uint8_t b = dmxData[base + 2];
            r = (r * dimmerFunc(globalDimmer)) / 255;
            g = (g * dimmerFunc(globalDimmer)) / 255;
            b = (b * dimmerFunc(globalDimmer)) / 255;
            leds->setPixelRGB(i, r, g, b, 0);
        }
        leds->show();
    }
    else
    {
        uint8_t offset = 5;
        uint16_t angle = 0;
        uint8_t speed = 250;
        for (int cycles = 0; cycles < 10000000; cycles++)
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                uint16_t h = (i * offset) + angle;
                leds->setPixelHSV16(i, h , 125, 255, 0);
            }
            leds->show();
            angle += speed;
        }
    }
}
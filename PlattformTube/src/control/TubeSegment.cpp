#include "drivers/ILEDDriver.h"
#include "control/TubeSegment.h"
#include "Arduino.h"

TubeSegment::TubeSegment()
{
}

TubeSegment::TubeSegment(int totalNumLeds, int startIdx, int endIdx, ILEDDriver *driver)
    : totalNumLeds(totalNumLeds), startIdx(startIdx), endIdx(endIdx), driver(driver)
{
}

void TubeSegment::loopWithDmx(SegmentValue segmentValues, DimmerCurve dimmerCurve)
{
    Serial.println("Flashing...");
    for (int i = 0; i < endIdx; i++)
    {
        driver->setPixelRGB(i, segmentValues.r, segmentValues.g, segmentValues.b, segmentValues.white);
    }
    driver->setBrightness(255);
    driver->show();
}
void TubeSegment::loopWithoutDmx(DimmerCurve dimmerCurve)
{
    uint8_t decay = 3;
    uint8_t brightness = 0;
    if (driver->getBrightness() > decay)
    {
        brightness = driver->getBrightness() - decay;
    }
    driver->setBrightness(brightness);
    driver->show();
}
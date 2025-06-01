#include "drivers/ILEDDriver.h"
#include "control/Segment.h"
#include "Arduino.h"

Segment::Segment()
{
}

Segment::Segment(int totalNumLeds, int startIdx, int endIdx)
    : totalNumLeds(totalNumLeds), startIdx(startIdx), endIdx(endIdx)
{
}

void Segment::print() {
    Serial.print("Start Index: ");
    Serial.print(startIdx);
    Serial.print(" End Index: ");
    Serial.println(endIdx);
    
}
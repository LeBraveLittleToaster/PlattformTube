#include "drivers/ILEDDriver.h"
#include "control/Segment.h"
#include "Arduino.h"

/**
 * @brief Default constructor for Segment.
 * 
 * Initializes an empty segment. Members are left uninitialized.
 * This could lead to undefined behavior unless explicitly set later.
 */
Segment::Segment()
{
    totalNumLeds = 0;
    startIdx = 0;
    endIdx = 0;
}

/**
 * @brief Constructs a Segment with start and end indices.
 * 
 * @param totalNumLeds Total number of LEDs in the driver (not used internally here).
 * @param startIdx Starting index of the segment (inclusive).
 * @param endIdx Ending index of the segment (exclusive or inclusive depending on usage).
 */
Segment::Segment(int totalNumLeds, int startIdx, int endIdx)
    : totalNumLeds(totalNumLeds), startIdx(startIdx), endIdx(endIdx)
{
    // This constructor sets all required fields.
}

/**
 * @brief Prints the segment's start and end indices via Serial.
 */
void Segment::print() {
    Serial.print("Start Index: ");
    Serial.print(startIdx);
    Serial.print(" End Index: ");
    Serial.println(endIdx);
}

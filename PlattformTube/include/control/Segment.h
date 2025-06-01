#include <drivers/ILEDDriver.h>

#pragma once

/**
 * @enum DimmerCurve
 * @brief Defines the dimming curve used to map input values to perceived brightness.
 * 
 * LINEAR: Linear brightness scaling.
 * S_LINEAR: Smooth linear (likely gamma-corrected or eased) brightness scaling.
 */
enum DimmerCurve {
    LINEAR,
    S_LINEAR
};

/**
 * @class Segment
 * @brief Represents a contiguous range of LEDs within a larger LED strip or array.
 * 
 * Defines the start and end indices of the segment and tracks the total number of LEDs.
 */
class Segment {
public:
    /**
     * @brief Default constructor.
     * 
     * Initializes all members to zero or default values.
     */
    Segment();

    /**
     * @brief Constructs a Segment covering LEDs from startIdx to endIdx.
     * 
     * @param totalNumLeds Total number of LEDs in the entire strip/array.
     * @param startIdx Starting LED index of this segment (inclusive).
     * @param endIdx Ending LED index of this segment (inclusive).
     */
    Segment(int totalNumLeds, int startIdx, int endIdx);

    /**
     * @brief Prints segment details for debugging.
     * 
     * Outputs totalNumLeds, startIdx, and endIdx values.
     */
    void print();

    int totalNumLeds; ///< Total number of LEDs in the entire strip.
    int startIdx;     ///< Start index of this segment (inclusive).
    int endIdx;       ///< End index of this segment (inclusive).
};

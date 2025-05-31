#include <drivers/ILEDDriver.h>

#pragma once

struct SegmentValue
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t white;
};

enum DimmerCurve {
    LINEAR,
    S_LINEAR
};


class Segment {
    public:
        Segment();
        Segment(int totalNumLeds, int startIdx, int endIdx, ILEDDriver *driver);
        void print();
        void loopWithDmx(SegmentValue segmentValues, DimmerCurve dimmerCurve);
        void loopWithoutDmx(DimmerCurve dimmerCurve);
    private:
        int totalNumLeds;
        int startIdx;
        int endIdx;
        ILEDDriver *driver;
};
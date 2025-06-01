#include <drivers/ILEDDriver.h>

#pragma once

enum DimmerCurve {
    LINEAR,
    S_LINEAR
};


class Segment {
    public:
        Segment();
        Segment(int totalNumLeds, int startIdx, int endIdx);
        void print();
        
        int totalNumLeds;
        int startIdx;
        int endIdx;
};
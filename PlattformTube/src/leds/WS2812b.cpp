#include "FastLED.h"

enum DimmerCurve {
    LINEAR,
    S_CURVE
};

class LedStateTracker {
    private:
        CRGB leds[numLeds];
        int numLeds;
    public:
        LedStateTracker(CRGB leds[]){
            
        }
};

class WS2812B_Type {
    private:
        LedStateTracker ledState;
        DimmerCurve dimmerCurve;
    public:
        WS2812B_Type (int numLeds, DimmerCurve dimmerCurve) : dimmerCurve(dimmerCurve) {

        }

    void oneShotValue() {

    }
};
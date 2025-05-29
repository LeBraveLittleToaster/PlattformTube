#ifndef TICKER_H
#define TICKER_H

#include <Arduino.h>

class TickController {
public:
    // Waits until the next tick and returns how long it delayed
    unsigned long delayTillNextTick();

private:
    unsigned long lastTimestamp = 0;
    static const unsigned long interval = 2273; // milliseconds (approx 44Hz)
};

#endif // TICKER_H
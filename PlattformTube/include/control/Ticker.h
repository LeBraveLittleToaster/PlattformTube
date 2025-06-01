#ifndef TICKER_H
#define TICKER_H

#include <Arduino.h>

class Ticker {
public:
    void start();
    // Waits until the next tick and returns how long it delayed
    unsigned long delayTillNextTick();
    unsigned long delayOneSecond();

private:
    unsigned long lastTimestamp = 0;
    static const unsigned long interval = 23; // milliseconds (approx 44Hz)
};

#endif // TICKER_H
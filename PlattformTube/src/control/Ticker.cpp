#include <Arduino.h>
#include "control/Ticker.h"

void Ticker::start()
{
    this->lastTimestamp = millis();
}
unsigned long Ticker::delayTillNextTick()
{
    unsigned long currentMillis = millis();
    unsigned long nextTickMillis = this->lastTimestamp + (this->interval);
    
    
    if (nextTickMillis < currentMillis)
    {
        this->lastTimestamp = nextTickMillis;
        return 0;
    }
    this->lastTimestamp = nextTickMillis;
    unsigned long delayMillis = nextTickMillis - currentMillis;
    
    delay(delayMillis);
    return delayMillis;
}
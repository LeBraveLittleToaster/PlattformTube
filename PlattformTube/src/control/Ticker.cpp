#include <Arduino.h>
#include "control/Ticker.h"

Ticker::Ticker(unsigned long interval) : interval(interval){}

/**
 * @brief Starts the ticker by recording the current time.
 */
void Ticker::start()
{
    this->lastTimestamp = millis();
}

/**
 * @brief Checks if it's time for the next tick.
 * 
 * Call this repeatedly (e.g., inside `loop()`), and when it returns true,
 * perform your tick logic. No blocking delay is used.
 * 
 * @return True if the interval has elapsed since the last tick.
 */
bool Ticker::isTickReady()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastTimestamp >= interval)
    {
        lastTimestamp += interval;
        return true;
    }
    return false;
}

/**
 * @brief Returns how long (in ms) until the next tick.
 * 
 * @return Milliseconds until the next scheduled tick.
 */
unsigned long Ticker::timeUntilNextTick()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastTimestamp >= interval)
    {
        return 0;
    }
    return interval - (currentMillis - lastTimestamp);
}

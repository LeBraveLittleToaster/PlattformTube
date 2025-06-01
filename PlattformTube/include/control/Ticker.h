#ifndef TICKER_H
#define TICKER_H

class Ticker
{
public:
    
    Ticker(unsigned long interval);

    void start();

    /**
     * Checks if the tick interval has elapsed.
     */
    bool isTickReady();

    /**
     * Returns remaining time until next tick.
     */
    unsigned long timeUntilNextTick();

private:
    /**
     * Set the interval between ticks (in milliseconds).
     */
    unsigned long interval = 1000;
    unsigned long lastTimestamp = 0;
};

#endif // TICKER_H

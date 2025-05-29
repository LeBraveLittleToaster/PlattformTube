#include <Arduino.h>

class TickController {
    public:
        unsigned long delayTillNextTick() {
            unsigned long currentMillis = millis();
            unsigned long nextTickMillis = this->lastTimestamp + this->interval;
            if(nextTickMillis < currentMillis){
                return 0;
            }
            this->lastTimestamp = currentMillis;
            unsigned long delayMillis = nextTickMillis - currentMillis;
            delay(delayMillis);
            return delayMillis;
        }
    
    private:
        unsigned long lastTimestamp = 0;
        const unsigned long interval = 2273; // 1000 / 44

};
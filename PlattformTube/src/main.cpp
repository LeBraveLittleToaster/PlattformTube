#include <Arduino.h>
#include "control/Lighttube.h"
#include "config/HardwareConfig.h"
#include "control/Ticker.h"
#include <Adafruit_NeoPixel.h>

#if defined(LED_DRIVER_WS2812)
#include "drivers/WS2812Driver.h"
WS2812Driver ledDriver(LED_DATA_PIN, NUM_LEDS);
#endif

#if defined(DMX_DRIVER_MAX485)
#include "drivers/DMXMAX485.h"
// Use the DMX pin definitions from HardwareConfig.h
DMXMAX485 dmxDriver(DMX_NUM_1, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN);
#endif

Ticker ticker;

Lighttube tube{&dmxDriver, &ledDriver, &ticker, 72};

void setup()
{
    delay(1000);
    Serial.begin(115200);
    tube.setup();
    tube.print();
    Serial.println("Starting Ticker...");
    ticker.start();
    dmxDriver.begin();
}

void loop()
{
    // Update DMX data
    dmxDriver.readData();

    // Main application logic
    tube.loop();

    // Optional: Add a small delay or use ticker for timing
    delay(10);
}
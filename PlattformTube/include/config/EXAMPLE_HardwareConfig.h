#ifndef HARDWARECONFIG_H
#define HARDWARECONFIG_H

//---------------------------------------------------------
// mDNS Settings
//---------------------------------------------------------

// Device name for mDNS (Multicast DNS) service
#define MDNS_DEVICE_NAME "ESP32Light.local"

//---------------------------------------------------------
// System timing
//---------------------------------------------------------

// Main update interval (milliseconds) for animation ticker
#define TICKER_INTERVAL_MILLIS 23


//---------------------------------------------------------
// LED configuration
//---------------------------------------------------------

// GPIO pin used for data output to the LED strip
#define LED_DATA_PIN 21

// Total number of addressable LED pixels in the fixture
#define NUM_LEDS 6

// Number of logical segments the LEDs are divided into
#define SEGMENTS 3


//---------------------------------------------------------
// DMX interface (RS-485) configuration
//---------------------------------------------------------

// UART RX pin for DMX input
#define DMX_RX_PIN 3

// UART TX pin for DMX output or thru
#define DMX_TX_PIN 20

// Enable pin for the RS-485 transceiver (DE/RE)
#define DMX_EN_PIN 19


//---------------------------------------------------------
// LED driver selection
// Only one driver should be enabled at a time.
//---------------------------------------------------------

#define LED_DRIVER_WS2812_RGB
// #define LED_DRIVER_TM1814_RGBW


//---------------------------------------------------------
// DMX transceiver chipset selection
//---------------------------------------------------------

#define DMX_DRIVER_MAX485


//---------------------------------------------------------
// Network configuration
//---------------------------------------------------------

// Wi-Fi SSID for the device to join
#define WLAN_SSID "adminadmin"

// Wi-Fi password for the network
#define WLAN_PASSWORD "adminadmin"


//---------------------------------------------------------
// Device security key
// Used for local authentication or API protection.
//---------------------------------------------------------

#define SECURE_KEY "12345"


#endif // HARDWARECONFIG_H

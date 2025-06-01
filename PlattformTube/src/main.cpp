// #include <Arduino.h>
// #include "control/Lighttube.h"
// #include "config/HardwareConfig.h"
// #include "control/Ticker.h"
// #include <Adafruit_NeoPixel.h>

// #if defined(LED_DRIVER_WS2812)
// #include "drivers/WS2812Driver.h"
// WS2812Driver ledDriver(LED_DATA_PIN, NUM_LEDS);
// #endif

// #if defined(DMX_DRIVER_MAX485)
// #include "drivers/DMXMAX485.h"
// DMXMAX485 dmxDriver(Serial1, DMX_RX_PIN);
// #endif
// #include <esp_dmx.h>
// #include <rdm/responder.h>

// Ticker ticker;

// Lighttube tube{&dmxDriver, &ledDriver, &ticker, 3};


#include <Arduino.h>
#include <esp_dmx.h>
#include <Adafruit_NeoPixel.h>

// DMX Definitions
#define DMX_CHANNELS 30
#define DMX_UNIVERSE_SIZE DMX_PACKET_SIZE

// LED Strip Definitions
#define LED_PIN     7
#define LED_COUNT   10

// DMX input pin (RX)
int transmitPin = -1;
int receivePin  = 3;   // UART RX
int enablePin   = -1;

dmx_port_t dmxPort = 1;
byte dmxData[DMX_UNIVERSE_SIZE];

bool dmxIsConnected = false;
unsigned long lastUpdate = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  delay(1000);
  Serial.begin(115200);
  while (!Serial);

  strip.begin();
  strip.show(); // Turn off all LEDs

  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_personality_t personalities[] = {
    {1, "WS2812B 10x RGB"}
  };

  dmx_driver_install(dmxPort, &config, personalities, 1);
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);

  Serial.println("DMX to WS2812B + Serial Debug initialized.");


  // Serial.begin(115200);
  // tube.setup();
  // tube.print();
  // Serial.println("Starting Ticker...");
  // ticker.start();

  // const dmx_port_t dmx_num = DMX_NUM_1;

  // // First, use the default DMX configuration...
  // dmx_config_t config = DMX_CONFIG_DEFAULT;

  // // ...declare the driver's DMX personalities...
  // const int personality_count = 1;
  // dmx_personality_t personalities[] = {
  //     {1, "Default Personality"}};

  // // ...install the DMX driver...
  // Serial.print("Installing DMX driver ");
  // Serial.println(dmx_driver_install(dmx_num, &config, personalities, personality_count));

  // // ...and then set the communication pins!
  // const int tx_pin = -1;
  // const int rx_pin = 10;
  // const int rts_pin = -1;
  // Serial.print("Setting DMX pins ");
  // Serial.println(dmx_set_pin(dmx_num, tx_pin, rx_pin, rts_pin));
}

void loop()
{
  dmx_packet_t packet;

  if (dmx_receive(dmxPort, &packet, DMX_TIMEOUT_TICK)) {
    unsigned long now = millis();

    if (!packet.err) {
      if (!dmxIsConnected) {
        Serial.println("DMX signal detected.");
        dmxIsConnected = true;
      }

      dmx_read(dmxPort, dmxData, packet.size);

      // Update LEDs
      for (int i = 0; i < LED_COUNT; i++) {
        int base = 1 + i * 3;
        if (base + 2 < DMX_UNIVERSE_SIZE) {
          uint8_t r = dmxData[base];
          uint8_t g = dmxData[base + 1];
          uint8_t b = dmxData[base + 2];
          strip.setPixelColor(i, strip.Color(r, g, b));
        }
      }
      strip.show();

      // Print values every second
      if (now - lastUpdate > 1000) {
        Serial.print("Start Code: 0x");
        Serial.println(dmxData[0], HEX);

        for (int i = 1; i <= DMX_CHANNELS; i++) {
          Serial.printf("Channel %2d: %3d\n", i, dmxData[i]);
        }
        Serial.println("-----------------------------");

        lastUpdate = now;
      }

    } else {
      Serial.println("DMX error occurred.");
    }

  } else if (dmxIsConnected) {
    Serial.println("DMX connection lost.");
    dmx_driver_delete(dmxPort);
    while (true) yield(); // Halt
  }



  // dmx_packet_t packet;
  // uint8_t data[512] = {0};
  // int size = dmx_receive(DMX_NUM_1, &packet, 5000);
  // if (size > 0) {
  //   dmx_read(DMX_NUM_1, data, size);
  // }
  // Serial.print("DMX Packet Size: ");
  // Serial.println(size);
  // Serial.print("DMX Packet Start Code: ");
  // Serial.println(packet.sc);
  // Serial.print("DMX Packet Error: ");
  // Serial.println(packet.err);

  // switch (packet.err) {
  //     case DMX_OK:
  //       printf("Received packet with start code: %02X and size: %i.\n",
  //         packet.sc, packet.size);
  //       // Data is OK. Now read the packet into the buffer.
  //       dmx_read(DMX_NUM_1, data, packet.size);
  //       break;
      
  //     case DMX_ERR_TIMEOUT:
  //       printf("The driver timed out waiting for the packet.\n");
  //       /* If the provided timeout was less than DMX_TIMEOUT_TICK, it may be
  //         worthwhile to call dmx_receive() again to see if the packet could be
  //         received. */
  //       break;

  //     case DMX_ERR_IMPROPER_SLOT:
  //       printf("Received malformed byte at slot %i.\n", packet.size);
  //       /* A slot in the packet is malformed. Data can be recovered up until 
  //         packet.size. */
  //       break;

  //     case DMX_ERR_UART_OVERFLOW:
  //       printf("The DMX port overflowed.\n");
  //       /* The ESP32 UART overflowed. This could occur if the DMX ISR is being
  //         constantly preempted. */
  //       break;
      
  //     default:
  //       printf("An unknown error occurred: %i.\n", packet.err);
  //       /* An unknown error occurred. */
  //       break;
  //   }
  // //tube.loop();
}

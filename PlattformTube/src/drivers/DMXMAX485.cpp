#include "drivers/DMXMAX485.h"

DMXMAX485::DMXMAX485(
    dmx_port_t dmxPort,
    int rxPin,
    int txPin,
    int enPin
) : dmxPort(dmxPort), rxPin(rxPin), txPin(txPin), enPin(enPin) {}

void DMXMAX485::begin() {
    dmx_config_t config = DMX_CONFIG_DEFAULT;
    dmx_personality_t personalities[] = {
        {1, "Default Personality"}
    };
    int personality_count = 1;

    dmx_driver_install(dmxPort, &config, personalities, personality_count);

    // Pins are defined in hardwareconfig.h or passed to constructor
    dmx_set_pin(dmxPort, txPin, rxPin, enPin);
  }

bool DMXMAX485::readData() {
    dmx_packet_t packet;
    if (dmx_receive(dmxPort, &packet, DMX_TIMEOUT_TICK)) {
        if (!packet.err) {
            dmx_read(dmxPort, dmxBuffer, packet.size);
            dmxIsConnected = true;
            return true;
        } else {
            dmxIsConnected = false;
        }
    }
    return false;
}

uint8_t* DMXMAX485::getBuffer(){
  for(int i = 0; i < DMX_BUFFER_SIZE-1; ++i) {
    dmxBuffer[i] = dmxBuffer[i+1]; 
  }
  return dmxBuffer;
}

uint8_t DMXMAX485::getChannel(int channel) {
    if (channel >= 0 && channel < DMX_BUFFER_SIZE) {
        return dmxBuffer[channel];
    }
    return 0;
}

DMXMAX485::~DMXMAX485() {
    dmx_driver_delete(dmxPort);
}
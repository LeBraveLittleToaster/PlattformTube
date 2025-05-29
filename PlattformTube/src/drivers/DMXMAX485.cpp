#include "DMXMAX485.h"

DMXMAX485::DMXMAX485(HardwareSerial& serial, int rxPin)
    : serial(serial), rxPin(rxPin) {}

void DMXMAX485::begin() {
    serial.begin(250000, SERIAL_8N2, rxPin, -1); // -1 for TX if not used
}

bool DMXMAX485::readData() {
  bool newData = false;
  int bytesRead = 0;

  // DMX frames are 513 bytes: 1 start code + 512 data bytes
  if (serial.available() >= 513) {
    // Read start code (should be 0 for DMX data)
    uint8_t startCode = serial.read();
    if (startCode == 0) {
      // Read 512 bytes into dmxBuffer
      for (int i = 0; i < 512; ++i) {
        if (serial.available()) {
          dmxBuffer[i] = serial.read();
          bytesRead++;
        } else {
          break;
        }
      }
      if (bytesRead == 512) {
        newData = true;
      }
    } else {
      // Skip the rest of the frame if start code is not 0
      for (int i = 0; i < 512 && serial.available(); ++i) {
        serial.read();
      }
    }
  }
  return newData;
}

const uint8_t* DMXMAX485::getBuffer() const {
    return dmxBuffer;
}

uint8_t DMXMAX485::getChannel(int channel){
    if (channel >= 0 && channel < DMX_BUFFER_SIZE) {
        return dmxBuffer[channel];
    }
    return 0;
}
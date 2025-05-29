#include "EffectManager.h"

// needs to be implemented
// do we want an effect Manger at all?

void EffectManager::setEffect(int id) {
  currentEffect = id;
}

// void EffectManager::applyEffect(ILEDDriver* driver) {
//   for (int i = 0; i < 60; i++) {
//     driver->setPixel(i, (i + millis() / 10) % 255 << 16); // red shifting
//   }
//   driver->show();
// }
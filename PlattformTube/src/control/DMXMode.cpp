#include "control/DMXMode.h"
#include <memory>

uint8_t getSegmentCount(DmxMode tv)
{
    switch (tv)
    {
    case DmxMode::DMX_1:  return 1;
    case DmxMode::DMX_5:  return 1;
    case DmxMode::DMX_30: return 6;
    case DmxMode::DMX_40: return 8;
    case DmxMode::DMX_80: return 16;
    default:              return 0;
    }
}
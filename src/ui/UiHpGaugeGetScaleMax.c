// bdc 0x0888a360 UiHpGaugeGetScaleMax
#include "bdc.h"

/* Returns the value that maps to a full bar for the HUD hit-point gauge (`UiHpGaugeInit`, 0xa0
   bytes): for a unit source (mode 1) the larger of the max HP `+0x70` and 1.5 × the stat
   `stats+0x90` (`unit+0x4bc`); 1000 for other modes. Used by `UiHpGaugeEmitBarSprite`. */

float UiHpGaugeGetScaleMax(UiHpGauge *self)
{
  float result = 1000.0f;
  if (self->mode == 1) {
    float limit = ((self->unit->combat).stats)->levelHp[4] * 1.5f;
    result = self->maxHp;
    if (!(result <= limit)) {
      return result;
    }
    result = limit;
  }
  return result;
}

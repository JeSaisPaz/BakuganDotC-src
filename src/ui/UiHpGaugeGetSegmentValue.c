// bdc 0x0888a31c UiHpGaugeGetSegmentValue
#include "bdc.h"

/* Returns the part of `value` that falls into bar segment `segment` of the HUD hit-point gauge
   (`UiHpGaugeInit`, 0xa0 bytes): `value − segment × max` (`max` = `+0x70`), clamped to 0..max.
   Used by `UiHpGaugeEmitBars` for HP above one bar length. */

float UiHpGaugeGetSegmentValue(float value, UiHpGauge *self, s32 segment)
{
  float max = self->maxHp;
  float result = value - (float)segment * max;
  if (!(result <= max)) {
    result = max;
  }
  if (result < 0.0f) {
    result = 0.0f;
  }
  return result;
}

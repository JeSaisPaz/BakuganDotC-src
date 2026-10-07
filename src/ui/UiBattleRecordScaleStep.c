// bdc 0x089483b8 UiBattleRecordScaleStep
#include "bdc.h"

/* Returns `(scale − 1) / frames · step + 1`: the scale at `step` of a linear ramp from 1 to
   `scale` over `frames` steps. */

float UiBattleRecordScaleStep(float scale, float frames, float step)

{
  return ((scale - 1.0f) / frames) * step + 1.0f;
}


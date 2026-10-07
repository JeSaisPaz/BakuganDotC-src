// bdc 0x0889d1f0 BtlStageUvAnimStepType2
#include "bdc.h"

/* UV animation step of types 2 and 4 (`BtlStageUpdateUvAnims`): adds `anim[1]` to the offset
   `anim[0]` and wraps it into 0..1 (smooth scrolling): +1 when below 0, then -1 unless below 1
   (so a NaN offset also gets -1, as in the compiled `c.lt.s`/`bc1t`). */

void BtlStageUvAnimStepType2(float *anim)
{
  anim[0] = anim[0] + anim[1];
  if (anim[0] < 0.0f) {
    anim[0] = anim[0] + 1.0f;
  }
  if (!(anim[0] < 1.0f)) {
    anim[0] = anim[0] - 1.0f;
  }
}

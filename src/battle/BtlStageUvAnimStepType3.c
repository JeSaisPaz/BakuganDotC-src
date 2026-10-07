// bdc 0x0889d250 BtlStageUvAnimStepType3
#include "bdc.h"

/* UV animation step of type 3 (`BtlStageUpdateUvAnims`): adds `anim[2]` to the offset `anim[0]`
   and wraps it into 0..1: +1 when below 0, then -1 unless below 1. */

void BtlStageUvAnimStepType3(float *anim)
{
  anim[0] = anim[0] + anim[2];
  if (anim[0] < 0.0f) {
    anim[0] = anim[0] + 1.0f;
  }
  if (!(anim[0] < 1.0f)) {
    anim[0] = anim[0] - 1.0f;
  }
}

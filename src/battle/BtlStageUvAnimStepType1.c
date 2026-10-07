// bdc 0x0889d168 BtlStageUvAnimStepType1
#include "bdc.h"

/* UV animation step of type 1 (`BtlStageUpdateUvAnims`): advances the frame accumulator `anim[1]`
   by `anim[3]`; each time it reaches 1.0 it subtracts 1.0 and steps the offset `anim[0]` by
   `anim[2]` (stepped scrolling), then wraps the offset once into 0..1 (+1.0 below 0, -1.0 at or
   above 1). */
void BtlStageUvAnimStepType1(float *anim)
{
    float frame = anim[1] + anim[3];

    anim[1] = frame;
    if (frame < 1.0f) {
        return;
    }
    anim[1] = frame - 1.0f;
    anim[0] = anim[0] + anim[2];
    if (anim[0] < 0.0f) {
        anim[0] = anim[0] + 1.0f;
    }
    if (!(anim[0] < 1.0f)) {
        anim[0] = anim[0] - 1.0f;
    }
}

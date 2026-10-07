// bdc 0x08846e90 BtlCameraCalcDefaultPitch
#include "bdc.h"

/* Camera pitch helper of `BtlCameraSetDefaultFollow`/`BtlCameraUpdateDefault`: computes
   `t = (BtlStageGetCeilingHeight() - height) * 0.001`, clamps it to [0, 1] and returns
   `basePitch + (1 - t) * (0.5236 rad (30°) - basePitch)`: the closer the ceiling is to `height`,
   the closer the pitch gets to 30°. */
float BtlCameraCalcDefaultPitch(float height, float basePitch)
{
    float ceiling;
    float t;

    ceiling = BtlStageGetCeilingHeight();
    t = (ceiling - height) * 0.00100000005f; /* 0x3a83126f */
    /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). +NaN loses vmin and gives 1,
       -NaN wins vmin and then loses vmax, giving 0 (VfpuLift semantics table: vmin/vmax). */
    if (t != t) {
        t = __builtin_signbit(t) ? 0.0f : 1.0f;
    } else {
        t = t <= 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
    }
    return basePitch + (1.0f - t) * (0.52359879f - basePitch); /* 0x3f060a92 */
}

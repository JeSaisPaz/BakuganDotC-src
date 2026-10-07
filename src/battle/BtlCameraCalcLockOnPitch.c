// bdc 0x08846f00 BtlCameraCalcLockOnPitch
#include "bdc.h"

/* Camera pitch helper of the lock-on modes (`BtlCameraUpdateLockOn`, `BtlCameraUpdateLockAim`):
   takes the height difference `dy = targetY - selfY`, clamps `dy * 0.01` to [0, 1], and blends
   `basePitch` toward 50° (0.8727 rad) by that factor times `weight`. When the target is below
   (`dy < 0`) it then tilts the pitch back toward -50° by `k * (basePitch + 50°)`, with
   `k = sqrt(-dy) * 0.05 * weight`, capped at `1.5 * weight` when `sqrt(-dy) * 0.05` exceeds 1.5. */
float BtlCameraCalcLockOnPitch(float selfY, float targetY, float basePitch, float weight)
{
    float dy = targetY - selfY;
    float t;
    float pitch;
    float s;
    float k;

    t = dy * 0.00999999978f; /* 0x3c23d70a */
    /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). +NaN loses vmin and gives 1,
       -NaN wins vmin and then loses vmax, giving 0 (VfpuLift semantics table: vmin/vmax). */
    if (t != t) {
        t = __builtin_signbit(t) ? 0.0f : 1.0f;
    } else {
        t = t <= 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
    }
    pitch = t * weight * (0.872664630f - basePitch) + basePitch; /* 0x3f5f66f3 */
    if (dy < 0.0f) {
        s = __builtin_sqrtf(-dy) * 0.0500000007f; /* 0x3d4ccccd */
        if (s <= 1.5f) {
            k = s * weight;
        } else {
            k = 1.5f * weight;
        }
        pitch -= k * (basePitch - -0.872664630f); /* 0xbf5f66f3 */
    }
    return pitch;
}

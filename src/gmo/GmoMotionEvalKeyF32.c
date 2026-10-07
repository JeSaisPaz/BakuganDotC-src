// bdc 0x089dd8e0 GmoMotionEvalKeyF32
#include "bdc.h"

/* Float-key counterpart of `GmoMotionEvalKeyF16` (called by `GmoMotionAdvance`): `t = (frame -
   t0)/(t1 - t0)` (0 when both keys are the same), then by the track mode `track->param8 & 0xf`:
   0 returns key0's value (`+0x4`), 1 linear `a + (b - a)*t`, 2 cubic (`GmoInterpCubicF32`),
   4 slerp (`GmoInterpSlerpF32`); any other mode (3 = Bezier) evaluates each of the 4 components
   only when `bezier` is set (parameter from `GmoBezierSolveParam`, then the Bernstein polynomial on
   the 0x14-byte keys) and returns the 4 results. With `bezier` clear the PSP leaves C000 untouched
   (stale); the C returns zeros there. Returns the value (left in VFPU C000 on the PSP). */

ScePspFVector4 GmoMotionEvalKeyF32(float frame, float t0, float t1, const GmoMotionTrack *track, const void *key0,
                                   const void *key1, bool bezier)
{
    const float *k0 = (const float *)key0;
    const float *k1 = (const float *)key1;
    float out[4];
    ScePspFVector4 r;
    float t;
    u32 mode;
    s32 i;

    if (key0 == key1) {
        t = 0.0f;
    } else {
        t = (frame - t0) / (t1 - t0);
    }
    mode = track->param8 & 0xf;
    if (mode == 0) {
        r.x = k0[1];
        r.y = k0[2];
        r.z = k0[3];
        r.w = k0[4];
        return r;
    }
    if (mode == 1) {
        r.x = k0[1] + (k1[1] - k0[1]) * t;
        r.y = k0[2] + (k1[2] - k0[2]) * t;
        r.z = k0[3] + (k1[3] - k0[3]) * t;
        r.w = k0[4] + (k1[4] - k0[4]) * t;
        return r;
    }
    if (mode == 2) {
        return GmoInterpCubicF32(t, (const GmoMotionKeyCubicF *)key0, (const GmoMotionKeyCubicF *)key1);
    }
    if (mode == 4) {
        return GmoInterpSlerpF32(t, (const GmoMotionKeyQuatF *)key0, (const GmoMotionKeyQuatF *)key1);
    }
    r.x = 0.0f;
    r.y = 0.0f;
    r.z = 0.0f;
    r.w = 0.0f;
    if (bezier) {
        for (i = 0; i < 4; i++) {
            float s = GmoBezierSolveParam(t0, t0 + k0[4], t1 + k1[2], t1, frame);
            float u = 1.0f - s;
            float s2 = s * s;
            float u2 = u * u;
            float b1 = u2 * s * 3.0f;
            float b2 = u * s2 * 3.0f;
            float b0 = u * u2 + b1;
            float b3 = s * s2 + b2;
            out[i] = k0[1] * b0 + k0[5] * b1 + k1[3] * b2 + k1[1] * b3;
            k0 += 5;
            k1 += 5;
        }
        r.x = out[0];
        r.y = out[1];
        r.z = out[2];
        r.w = out[3];
    }
    return r;
}

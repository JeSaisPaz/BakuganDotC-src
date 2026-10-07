// bdc 0x089ddb98 GmoMotionEvalKeyF16
#include "bdc.h"

/* Half-float counterpart of `GmoMotionEvalKeyF32` (called by `GmoMotionAdvance`): `t = (frame -
   t0) / (t1 - t0)` (0 when both keys are the same), then by the track mode `track->param8 & 0xf`:
   0 returns key0's 4 half values (`+0x2`, unaligned) converted with `vh2f`, 1 linear
   `a + (b - a)*t` on the converted halves, 2 cubic (`GmoInterpCubicF16`), 4 slerp
   (`GmoInterpSlerpF16`); any other mode (3 = Bezier) evaluates each of the 4 components only when
   `flag` is set (parameter from `GmoBezierSolveParam`, then the Bernstein polynomial on the
   0xa-byte keys, values converted with `GmoHalfToFloatBits`) and returns the 4 results. With
   `flag` clear the PSP leaves C000 untouched (stale); the C returns zeros there. Returns the value
   (left in VFPU C000 on the PSP). */

ScePspFVector4 GmoMotionEvalKeyF16(float frame, float t0, float t1, const GmoMotionTrack *track, const u16 *key0,
                                   const u16 *key1, bool flag)
{
    const u16 *k0 = key0;
    const u16 *k1 = key1;
    float out[4];
    ScePspFVector4 r;
    float a[4];
    float b[4];
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
        r.x = VfH2f(k0[1]);
        r.y = VfH2f(k0[2]);
        r.z = VfH2f(k0[3]);
        r.w = VfH2f(k0[4]);
        return r;
    }
    if (mode == 1) {
        for (i = 0; i < 4; i++) {
            a[i] = VfH2f(k0[i + 1]);
            b[i] = VfH2f(k1[i + 1]);
        }
        r.x = a[0] + (b[0] - a[0]) * t;
        r.y = a[1] + (b[1] - a[1]) * t;
        r.z = a[2] + (b[2] - a[2]) * t;
        r.w = a[3] + (b[3] - a[3]) * t;
        return r;
    }
    if (mode == 2) {
        return GmoInterpCubicF16(t, (const GmoMotionKeyCubicH *)key0, (const GmoMotionKeyCubicH *)key1);
    }
    if (mode == 4) {
        return GmoInterpSlerpF16(t, (const GmoMotionKeyQuatH *)key0, (const GmoMotionKeyQuatH *)key1);
    }
    r.x = 0.0f;
    r.y = 0.0f;
    r.z = 0.0f;
    r.w = 0.0f;
    if (flag) {
        for (i = 0; i < 4; i++) {
            float c0 = t0 + GmoHalfToFloatBits(k0[4]);
            float c1 = t1 + GmoHalfToFloatBits(k1[2]);
            float s = GmoBezierSolveParam(t0, c0, c1, t1, frame);
            float u = 1.0f - s;
            float s2 = s * s;
            float u2 = u * u;
            float b1 = u2 * s * 3.0f;
            float b2 = u * s2 * 3.0f;
            float b0 = u * u2 + b1;
            float b3 = s * s2 + b2;
            float v;

            v = GmoHalfToFloatBits(k0[1]) * b0;
            v = v + GmoHalfToFloatBits(k0[5]) * b1;
            v = v + GmoHalfToFloatBits(k1[3]) * b2;
            out[i] = v + GmoHalfToFloatBits(k1[1]) * b3;
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

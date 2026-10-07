// bdc 0x0881d46c GfxEffectRandomPointInShell
#include "bdc.h"

/* 3-D variant of `GfxEffectRandomPointInRing`: picks two random angles `a`, `b` in
   [-π, span − π) (`span · u − π` with u in [0, 1); `span` 2π when it is 0, giving [-π, π)) and a
   radius `r = sqrt((rMax − rMin)²·u + rMin²)`, and writes the spherical-coordinate point to `out`
   (x = sin a·cos b·r, y = cos a·cos b·r, z = sin b·r). Used by `GfxEffectEmitChildren`.
   The uniform samples come from the VFPU generator (`vrndf1.s` in [1, 2) minus the bank's 1.0);
   the trig is `vsin.s`/`vcos.s` of the angle times the bank's 2/π, i.e. sin/cos in radians. */
void GfxEffectRandomPointInShell(float rMin, float rMax, float span, float *out)
{
    float u0, u1, u2, angA, angB, d, rad, cosB;

    if (span == 0.0f) {
        span = 6.28318548f;
    }
    d = rMax - rMin;
    u0 = PlatformRandFloat12() - 1.0f;
    angA = span * u0 - 3.14159274f;
    u1 = PlatformRandFloat12() - 1.0f;
    angB = span * u1 - 3.14159274f;
    d = d * d;
    u2 = PlatformRandFloat12() - 1.0f;
    d = d * u2;
    rad = __builtin_sqrtf(d + rMin * rMin);
    out[2] = __builtin_sinf(angB) * rad;
    cosB = __builtin_cosf(angB) * rad;
    out[0] = __builtin_sinf(angA) * cosB;
    out[1] = __builtin_cosf(angA) * cosB;
}

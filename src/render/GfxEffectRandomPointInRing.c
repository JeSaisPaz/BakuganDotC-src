// bdc 0x0881d3a4 GfxEffectRandomPointInRing
#include "bdc.h"

/* Writes to `out` a random point in the XY plane at radius `r = sqrt(rMin² + (rMax − rMin)²·u)` (u
   uniform in [0,1); area-uniform in the ring only when rMin is 0) and angle `span·u′ − π`, i.e. in
   [−π, span − π) — centred on 0 only when span is 2π (used when `span` is 0); z = 0. Random draws
   use the VFPU generator (`vrndf1` minus 1.0); x = sin(angle)·r, y = cos(angle)·r. Used by
   `GfxEffectEmitChildren` to scatter child particles. */

void GfxEffectRandomPointInRing(float rMin, float rMax, float span, float *out)
{
    float width;
    float ang;
    float rad;

    if (span == 0.0f) {
        span = 6.28318548f;
    }
    width = rMax - rMin;
    ang = span * (PlatformRandFloat12() - 1.0f);
    ang = ang - 3.14159274f;
    width = width * width;
    width = width * (PlatformRandFloat12() - 1.0f);
    rad = __builtin_sqrtf(width + rMin * rMin);
    out[0] = __builtin_sinf(ang) * rad;
    out[1] = __builtin_cosf(ang) * rad;
    out[2] = 0.0f;
}

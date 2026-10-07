// bdc 0x089e8eac SegmentClosestParam
#include "bdc.h"

/* Parameter `t` in [0, 1] of the point on a `SegmentShape` closest to `p`:
   `t = clamp(-(dir . (start - p)) / (dir . dir), 0, 1)` over x, y, z. */
float SegmentClosestParam(SegmentShape *segment, const float *p)
{
    float toStart[3];
    float dirLenSq;
    float proj;
    float t;

    toStart[0] = segment->start[0] - p[0];
    toStart[1] = segment->start[1] - p[1];
    toStart[2] = segment->start[2] - p[2];
    dirLenSq = segment->dir[0] * segment->dir[0] + segment->dir[1] * segment->dir[1] +
               segment->dir[2] * segment->dir[2];
    proj = segment->dir[0] * toStart[0] + segment->dir[1] * toStart[1] + segment->dir[2] * toStart[2];
    t = -proj / dirLenSq;
    /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f). A NaN (dir of length 0): +NaN loses
       vmin and gives 1, -NaN wins vmin and then loses vmax, giving 0 (semantics table: vmin/vmax). */
    if (t != t)
        return __builtin_signbit(t) ? 0.0f : 1.0f;
    return t <= 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
}

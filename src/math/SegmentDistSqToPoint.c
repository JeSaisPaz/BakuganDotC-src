// bdc 0x089e8720 SegmentDistSqToPoint
#include "bdc.h"

static inline float Dot3(const float *a, const float *b)
{
    return VfDot3(a[0], a[1], a[2], b[0], b[1], b[2]); /* vdot.t */
}

/* Squared distance from point `p` to a `SegmentShape`. With v = p - start and d = dir:
   `v.d <= 0` gives `|v|^2`, `v.d < |d|^2` gives the perpendicular distance
   `|v|^2 - (v.d)^2 / |d|^2`, otherwise (also for NaN) `|v - d|^2`. */
float SegmentDistSqToPoint(SegmentShape *segment, const float *p)
{
    float v[3];
    float vMinusDir[3];
    float proj;
    float dirLenSq;

    v[0] = VfSub(p[0], segment->start[0]);
    v[1] = VfSub(p[1], segment->start[1]);
    v[2] = VfSub(p[2], segment->start[2]);
    vMinusDir[0] = VfSub(v[0], segment->dir[0]);
    vMinusDir[1] = VfSub(v[1], segment->dir[1]);
    vMinusDir[2] = VfSub(v[2], segment->dir[2]);

    proj = Dot3(v, segment->dir);
    if (proj <= 0.0f) {
        return Dot3(v, v);
    }
    dirLenSq = Dot3(segment->dir, segment->dir);
    if (proj < dirLenSq) {
        return Dot3(v, v) - (proj * proj) / dirLenSq;
    }
    return Dot3(vMinusDir, vMinusDir);
}

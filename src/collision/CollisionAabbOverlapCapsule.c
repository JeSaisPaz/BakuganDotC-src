// bdc 0x089e9880 CollisionAabbOverlapCapsule
#include "bdc.h"

/* Separating-axis test of a capsule shape against the axis-aligned box `aabb` (min at [0], max at
   [4]) grown by the capsule radius on every side; the capsule axis is then tested as a segment
   (`start`, vector `axis`) exactly like `CollisionAabbOverlapSegment`: doubled units
   (size = hi - lo, mid = 2*start + axis - lo - hi), the three box axes, then the three cross axes
   with |axis| padded by 0.0001. Returns true only if no axis separates them, false otherwise. */

bool CollisionAabbOverlapCapsule(const float *aabb, const void *query)
{
    const CollisionCapsule *c = query;
    float lo[3], hi[3];
    float size[3];
    float mid[3];
    float adx, ady, adz;
    int i;

    for (i = 0; i < 3; i++) {
        lo[i] = aabb[i] - c->radius;
        hi[i] = aabb[4 + i] + c->radius;
        size[i] = hi[i] - lo[i];
    }
    for (i = 0; i < 3; i++)
        mid[i] = c->start[i] * 2.0f + c->axis[i] - lo[i] - hi[i];

    adx = __builtin_fabsf(c->axis[0]);
    if (!(__builtin_fabsf(mid[0]) <= adx + size[0]))
        return false;
    ady = __builtin_fabsf(c->axis[1]);
    if (!(__builtin_fabsf(mid[1]) <= ady + size[1]))
        return false;
    adz = __builtin_fabsf(c->axis[2]);
    if (!(__builtin_fabsf(mid[2]) <= adz + size[2]))
        return false;

    adx += 0.0001f;
    ady += 0.0001f;
    adz += 0.0001f;
    if (!(__builtin_fabsf(c->axis[2] * mid[1] - c->axis[1] * mid[2]) <= adz * size[1] + ady * size[2]))
        return false;
    if (!(__builtin_fabsf(c->axis[0] * mid[2] - c->axis[2] * mid[0]) <= adz * size[0] + adx * size[2]))
        return false;
    if (!(__builtin_fabsf(c->axis[1] * mid[0] - c->axis[0] * mid[1]) <= ady * size[0] + adx * size[1]))
        return false;
    return true;
}

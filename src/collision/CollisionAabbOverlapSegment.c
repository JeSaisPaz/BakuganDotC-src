// bdc 0x089e9a0c CollisionAabbOverlapSegment
#include "bdc.h"

/* Separating-axis test of a segment shape (`start`, `dir`) against the axis-aligned box `aabb`
   (min at [0], max at [4]). Works in doubled units: `size` is max - min and `mid` is
   2*start + dir - min - max (twice the segment midpoint minus twice the box centre). Tests the
   three box axes, then the three cross axes dir × box-axis with |dir| padded by 0.0001. Returns
   true only if no axis separates them, false otherwise. */

bool CollisionAabbOverlapSegment(const float *aabb, const void *seg)
{
    const SegmentShape *s = seg;
    float size[3];
    float mid[3];
    float adx, ady, adz;
    int i;

    for (i = 0; i < 3; i++) {
        size[i] = aabb[4 + i] - aabb[i];
        mid[i] = s->start[i] * 2.0f + s->dir[i] - aabb[i] - aabb[4 + i];
    }

    adx = __builtin_fabsf(s->dir[0]);
    if (!(__builtin_fabsf(mid[0]) <= adx + size[0]))
        return false;
    ady = __builtin_fabsf(s->dir[1]);
    if (!(__builtin_fabsf(mid[1]) <= ady + size[1]))
        return false;
    adz = __builtin_fabsf(s->dir[2]);
    if (!(__builtin_fabsf(mid[2]) <= adz + size[2]))
        return false;

    adx += 0.0001f;
    ady += 0.0001f;
    adz += 0.0001f;
    if (!(__builtin_fabsf(s->dir[2] * mid[1] - s->dir[1] * mid[2]) <= adz * size[1] + ady * size[2]))
        return false;
    if (!(__builtin_fabsf(s->dir[0] * mid[2] - s->dir[2] * mid[0]) <= adz * size[0] + adx * size[2]))
        return false;
    if (!(__builtin_fabsf(s->dir[1] * mid[0] - s->dir[0] * mid[1]) <= ady * size[0] + adx * size[1]))
        return false;
    return true;
}

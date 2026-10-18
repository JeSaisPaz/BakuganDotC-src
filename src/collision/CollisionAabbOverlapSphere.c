// bdc 0x089e9828 CollisionAabbOverlapSphere
#include "bdc.h"

/* Returns whether the sphere of `query` (`CollisionSphereQuery`: `center`, `radius`) overlaps the
   axis-aligned box `aabb` (min, max): the per-axis distance from the centre to the box, clamped at 0
   (`vmax.t` against the bank zero vector C720), squared and summed, against radius². */

bool CollisionAabbOverlapSphere(const float *aabb, const void *query)
{
    const CollisionSphereQuery *q = query;
    float below[3];
    float above[3];
    float d[3];
    float distSq;
    float radius;

    below[0] = VfSub(aabb[0], q->center.x);
    below[1] = VfSub(aabb[1], q->center.y);
    below[2] = VfSub(aabb[2], q->center.z);
    above[0] = VfSub(q->center.x, aabb[4]);
    above[1] = VfSub(q->center.y, aabb[5]);
    above[2] = VfSub(q->center.z, aabb[6]);
    /* vmax.t against C720 (0.0f); a tie returns the second operand, +0.0f */
    below[0] = VfMax(below[0], 0.0f);
    below[1] = VfMax(below[1], 0.0f);
    below[2] = VfMax(below[2], 0.0f);
    above[0] = VfMax(above[0], 0.0f);
    above[1] = VfMax(above[1], 0.0f);
    above[2] = VfMax(above[2], 0.0f);
    d[0] = VfAdd(above[0], below[0]);
    d[1] = VfAdd(above[1], below[1]);
    d[2] = VfAdd(above[2], below[2]);
    distSq = VfDot3(d[0], d[1], d[2], d[0], d[1], d[2]);
    radius = q->radius;
    return distSq <= radius * radius;
}

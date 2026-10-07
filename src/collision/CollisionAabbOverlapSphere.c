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

    below[0] = aabb[0] - q->center.x;
    below[1] = aabb[1] - q->center.y;
    below[2] = aabb[2] - q->center.z;
    above[0] = q->center.x - aabb[4];
    above[1] = q->center.y - aabb[5];
    above[2] = q->center.z - aabb[6];
    /* vmax.t against C720 (0.0f); a tie returns the second operand, +0.0f */
    below[0] = below[0] > 0.0f ? below[0] : 0.0f;
    below[1] = below[1] > 0.0f ? below[1] : 0.0f;
    below[2] = below[2] > 0.0f ? below[2] : 0.0f;
    above[0] = above[0] > 0.0f ? above[0] : 0.0f;
    above[1] = above[1] > 0.0f ? above[1] : 0.0f;
    above[2] = above[2] > 0.0f ? above[2] : 0.0f;
    d[0] = above[0] + below[0];
    d[1] = above[1] + below[1];
    d[2] = above[2] + below[2];
    distSq = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    radius = q->radius;
    return distSq <= radius * radius;
}

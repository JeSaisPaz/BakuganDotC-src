// bdc 0x089e9508 CollisionCapsuleVsSegment
#include "bdc.h"

/* Capsule shape method for segment queries (type-4 slot 1): closest points between the capsule
   axis and the segment (`CollisionSegmentSegmentDistSq`). Returns false when that squared distance exceeds
   radius + radius (the game compares distSq against 2*radius, not a squared radius). Otherwise
   `out` = the axis-side closest point plus the saturated unit direction toward the segment-side
   point scaled by distSq (not the distance), `out->w` = 0, and returns true. */

bool CollisionCapsuleVsSegment(const void *capsuleArg, const void *seg, ScePspFVector4 *out)
{
    const CollisionCapsule *capsule = capsuleArg;
    float distSq = CollisionSegmentSegmentDistSq(&capsule->segmentHead, seg);
    ScePspFVector4 p0;
    ScePspFVector4 p1;
    float dx;
    float dy;
    float dz;
    float lenSq;
    float inv;

    if (capsule->radius + capsule->radius < distSq) {
        return false;
    }
    p0 = g_collisionClosestPoints[0];
    p1 = g_collisionClosestPoints[1];
    dx = p1.x - p0.x;
    dy = p1.y - p0.y;
    dz = p1.z - p0.z;
    lenSq = dx * dx + dy * dy + dz * dz;
    /* vcmp EZ + vcmovt: bank constant S713 (0.0f) replaces the inverse length of a zero vector. */
    if (lenSq == 0.0f) {
        inv = 0.0f;
    } else {
        inv = VfRsq(lenSq);
    }
    /* vpfxd [-1:1,-1:1,-1:1,M] + vscl.t, then vscl.t into C710 whose lane 3 is S713 (0.0f). */
    out->x = VfSat1(dx * inv) * distSq;
    out->y = VfSat1(dy * inv) * distSq;
    out->z = VfSat1(dz * inv) * distSq;
    out->w = 0.0f;
    out->x = out->x + g_collisionClosestPoints[0].x;
    out->y = out->y + g_collisionClosestPoints[0].y;
    out->z = out->z + g_collisionClosestPoints[0].z;
    return true;
}

// bdc 0x089e9448 CollisionCapsuleVsRay
#include "bdc.h"

/* Capsule shape method for ray queries (type-4 slot 0): closest points between the capsule
   axis and the ray (`CollisionRaySegmentDistSq`). Returns false when that squared distance exceeds
   radius + radius (the game compares distSq against 2*radius, not a squared radius). Otherwise
   `out` = the axis-side closest point plus the saturated unit direction toward the ray-side
   point scaled by distSq (not the distance), `out->w` = 0, and returns true. */

bool CollisionCapsuleVsRay(const void *capsuleArg, const void *ray, ScePspFVector4 *out)
{
    const CollisionCapsule *capsule = capsuleArg;
    float distSq = CollisionRaySegmentDistSq(&capsule->segment, ray);
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
    dx = VfSub(p1.x, p0.x);
    dy = VfSub(p1.y, p0.y);
    dz = VfSub(p1.z, p0.z);
    lenSq = VfDot3(dx, dy, dz, dx, dy, dz);
    /* vcmp EZ + vcmovt: bank constant S713 (0.0f) replaces the inverse length of a zero vector. */
    if (VfIsZero(lenSq)) {
        inv = 0.0f;
    } else {
        inv = VfRsq(lenSq);
    }
    /* vpfxd [-1:1,-1:1,-1:1,M] + vscl.t, then vscl.t into C710 whose lane 3 is S713 (0.0f). */
    out->x = VfMul(VfSat1(VfMul(dx, inv)), distSq);
    out->y = VfMul(VfSat1(VfMul(dy, inv)), distSq);
    out->z = VfMul(VfSat1(VfMul(dz, inv)), distSq);
    out->w = 0.0f;
    out->x = VfAdd(out->x, g_collisionClosestPoints[0].x);
    out->y = VfAdd(out->y, g_collisionClosestPoints[0].y);
    out->z = VfAdd(out->z, g_collisionClosestPoints[0].z);
    return true;
}

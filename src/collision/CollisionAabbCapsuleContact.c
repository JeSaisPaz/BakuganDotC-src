// bdc 0x089ea0e8 CollisionAabbCapsuleContact
#include "bdc.h"

/* If the capsule overlaps the AABB (`CollisionAabbOverlapCapsule`), writes the box centre into
   g_collisionAabbCenterTmp (an inlined `CollisionAabbCenter`), finds the point of the capsule axis
   (`segment`: start/axis) closest to it (`CollisionSegmentClosestPoint`) and writes to `out`
   the contact point on the capsule surface toward the box:
   `closest + normalize(boxCentre - closest) * radius` (a zero-length direction gives a zero offset,
   `out` = `closest`). Returns the overlap result; `out` is untouched on a miss. */

bool CollisionAabbCapsuleContact(const float *aabb, const void *capsule, ScePspFVector4 *out)
{
    const CollisionCapsule *c = capsule;
    ScePspFVector4 closest;
    ScePspFVector4 diff;
    float t;
    float lenSq;
    float scale;
    const ScePspFVector4 *center;
    bool hit;

    hit = CollisionAabbOverlapCapsule(aabb, capsule);
    if (hit) {
        /* Inlined centre: (min + max) * 0.5f; w is the bank zero S713 (vscl.t into C710). */
        g_collisionAabbCenterTmp.x = VfMul(VfAdd(aabb[0], aabb[4]), 0.5f);
        g_collisionAabbCenterTmp.y = VfMul(VfAdd(aabb[1], aabb[5]), 0.5f);
        g_collisionAabbCenterTmp.z = VfMul(VfAdd(aabb[2], aabb[6]), 0.5f);
        g_collisionAabbCenterTmp.w = 0.0f;
        CollisionSegmentClosestPoint(&c->segment, &g_collisionAabbCenterTmp, &t, &closest);
        center = CollisionAabbCenter((const ScePspFVector4 *)aabb);
        diff.x = VfSub(center->x, closest.x);
        diff.y = VfSub(center->y, closest.y);
        diff.z = VfSub(center->z, closest.z);
        lenSq = VfDot3(diff.x, diff.y, diff.z, diff.x, diff.y, diff.z);
        /* vcmp EZ + vcmovt: a zero length takes the bank zero S713 instead of 1/sqrt. */
        scale = VfRsq(lenSq);
        if (VfIsZero(lenSq))
            scale = 0.0f;
        scale = VfMul(scale, c->radius);
        out->x = VfAdd(VfMul(diff.x, scale), closest.x);
        out->y = VfAdd(VfMul(diff.y, scale), closest.y);
        out->z = VfAdd(VfMul(diff.z, scale), closest.z);
        /* w: S713 (0.0f) through vscl.t C710. */
        out->w = 0.0f;
    }
    return hit;
}

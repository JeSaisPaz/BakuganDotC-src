// bdc 0x089e95c8 CollisionCapsuleVsSphere
#include "bdc.h"

/* Capsule shape method for sphere queries (type-4 slot 2): closest axis point to the sphere centre;
   on overlap `out` is the contact point between the two surfaces (w = 0). Returns whether they overlap; `out` is left untouched otherwise.
   A zero-length axis->centre direction scales by the VFPU bank's 0 (S713), so `out` is then the closest axis point. */

bool CollisionCapsuleVsSphere(const void *capsuleArg, const void *sphereArg, ScePspFVector4 *out)
{
    const CollisionCapsule *capsule = capsuleArg;
    const CollisionSphere *sphere = sphereArg;
    float distSq;
    float sum;
    bool hit;

    distSq = CollisionSegmentClosestPoint(&capsule->segment, (const ScePspFVector4 *)sphere->center,
                                          g_collisionClosestParams, g_collisionClosestPoints);
    sum = capsule->radius + sphere->radius;
    hit = distSq <= sum * sum;
    if (hit) {
        float push = capsule->radius + (__builtin_sqrtf(distSq) - sum) * 0.5f;
        float dx = VfSub(sphere->center[0], g_collisionClosestPoints[0].x);
        float dy = VfSub(sphere->center[1], g_collisionClosestPoints[0].y);
        float dz = VfSub(sphere->center[2], g_collisionClosestPoints[0].z);
        float lenSq = VfDot3(dx, dy, dz, dx, dy, dz);
        float inv;

        /* vcmp EZ + vcmovt: a zero length takes the bank's S713 = 0 */
        if (VfIsZero(lenSq))
            inv = 0.0f;
        else
            inv = VfRsq(lenSq);
        /* sv.q C710: lanes x..z scaled, lane w is the bank's S713 = 0 (vadd.t below keeps it) */
        out->x = VfMul(VfSat1(VfMul(dx, inv)), push);
        out->y = VfMul(VfSat1(VfMul(dy, inv)), push);
        out->z = VfMul(VfSat1(VfMul(dz, inv)), push);
        out->w = 0.0f;
        out->x = VfAdd(out->x, g_collisionClosestPoints[0].x);
        out->y = VfAdd(out->y, g_collisionClosestPoints[0].y);
        out->z = VfAdd(out->z, g_collisionClosestPoints[0].z);
    }
    return hit;
}

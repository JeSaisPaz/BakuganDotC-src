// bdc 0x089e95c8 CollisionCapsuleVsSphere
#include "bdc.h"

/* Capsule shape method for sphere queries (type-4 slot 2): closest axis point to the sphere centre;
   on overlap `out` is the contact point between the two surfaces. Returns whether they overlap; `out` is left untouched otherwise.
   A zero-length axis->centre direction scales by the VFPU bank's 0 (S713), so `out` is then the closest axis point. */

bool CollisionCapsuleVsSphere(const void *capsuleArg, const void *sphereArg, ScePspFVector4 *out)
{
    const CollisionCapsule *capsule = capsuleArg;
    const CollisionSphere *sphere = sphereArg;
    float distSq;
    float sum;
    bool hit;

    distSq = CollisionSegmentClosestPoint(&capsule->segmentHead, (const ScePspFVector4 *)sphere->center,
                                          g_collisionClosestParams, g_collisionClosestPoints);
    sum = capsule->radius + sphere->radius;
    hit = distSq <= sum * sum;
    if (hit) {
        float push = capsule->radius + (__builtin_sqrtf(distSq) - sum) * 0.5f;
        float dx = sphere->center[0] - g_collisionClosestPoints[0].x;
        float dy = sphere->center[1] - g_collisionClosestPoints[0].y;
        float dz = sphere->center[2] - g_collisionClosestPoints[0].z;
        float lenSq = dx * dx + dy * dy + dz * dz;
        float inv;

        /* vcmp EZ + vcmovt: a zero length takes the bank's S713 = 0 */
        if (lenSq == 0.0f)
            inv = 0.0f;
        else
            inv = VfRsq(lenSq);
        /* out.w (stale VFPU lane S731) is not modelled */
        out->x = VfSat1(dx * inv) * push;
        out->y = VfSat1(dy * inv) * push;
        out->z = VfSat1(dz * inv) * push;
        out->x = out->x + g_collisionClosestPoints[0].x;
        out->y = out->y + g_collisionClosestPoints[0].y;
        out->z = out->z + g_collisionClosestPoints[0].z;
    }
    return hit;
}

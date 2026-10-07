// bdc 0x089e9d58 CollisionBvhTestRay
#include "bdc.h"

/* Ray/AABB test used for BVH culling: vertical rays (dir.x and dir.z both bit-zero) take a fast
   path that only checks the X/Z footprint and the Y interval, leaving the entry parameter in
   `g_collisionClosestParams`[0]; others go through `CollisionAabbRaySlab` with tmax 1e15. */

bool CollisionBvhTestRay(const float *aabb, const void *ray)
{
    const struct CollisionRayShape *r = (const struct CollisionRayShape *)ray;
    union { float f; u32 u; } dx, dz;
    float tNear;
    float tFar;
    float tMin;
    float tMax;

    dx.f = r->dir.x;
    dz.f = r->dir.z;
    if ((dx.u | dz.u) != 0) {
        return CollisionAabbRaySlab(1e15f, aabb, ray);
    }
    if (r->origin.x < aabb[0]) {
        return false;
    }
    if (!(r->origin.x <= aabb[4])) {
        return false;
    }
    if (r->origin.z < aabb[2]) {
        return false;
    }
    if (!(r->origin.z <= aabb[6])) {
        return false;
    }
    tNear = (aabb[1] - r->origin.y) * r->invDir.y;
    tFar = (aabb[5] - r->origin.y) * r->invDir.y;
    tMin = 0.0f;
    if (!(tNear <= 0.0f)) {
        tMin = tNear;
    }
    /* Not a clamp to a minimum: the asm keeps the larger of tFar and 1e15. */
    tMax = 1e15f;
    if (!(tFar <= 1e15f)) {
        tMax = tFar;
    }
    if (tMax < tMin) {
        return false;
    }
    g_collisionClosestParams[0] = tMin;
    return true;
}

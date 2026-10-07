// bdc 0x089e9b7c CollisionAabbRaySlab
#include "bdc.h"

/* Slab test of a ray (`CollisionRayShape`) against the AABB `aabb` (min at [0], max at [4]).
   An axis whose |dir| is below 0.0001 (checked X, Z, Y) rejects when `origin` lies outside that
   slab. Then, per axis X, Z, Y: lo = (min - origin) * invDir, hi = (max - origin) * invDir;
   tmin (starting at 0) is raised to lo and `tmax` is raised to hi, and the ray is rejected when
   tmax < tmin. On a hit tmin is stored in g_collisionClosestParams[0] and true is returned. */

bool CollisionAabbRaySlab(float tmax, const float *aabb, const void *ray)
{
    const CollisionRayShape *r = ray;
    float tmin;
    float lo0, lo1, lo2;
    float hi0, hi1, hi2;

    if (__builtin_fabsf(r->dir.x) < 0.0001f) {
        if (r->origin.x < aabb[0])
            return false;
        if (!(r->origin.x <= aabb[4]))
            return false;
    }
    if (__builtin_fabsf(r->dir.z) < 0.0001f) {
        if (r->origin.z < aabb[2])
            return false;
        if (!(r->origin.z <= aabb[6]))
            return false;
    }
    if (__builtin_fabsf(r->dir.y) < 0.0001f) {
        if (r->origin.y < aabb[1])
            return false;
        if (!(r->origin.y <= aabb[5]))
            return false;
    }
    tmin = 0.0f;

    /* vsub.t/vmul.t: per-axis slab entry/exit parameters. */
    lo0 = (aabb[0] - r->origin.x) * r->invDir.x;
    lo1 = (aabb[1] - r->origin.y) * r->invDir.y;
    lo2 = (aabb[2] - r->origin.z) * r->invDir.z;
    hi0 = (aabb[4] - r->origin.x) * r->invDir.x;
    hi1 = (aabb[5] - r->origin.y) * r->invDir.y;
    hi2 = (aabb[6] - r->origin.z) * r->invDir.z;

    if (!(lo0 <= tmin))
        tmin = lo0;
    if (!(hi0 <= tmax))
        tmax = hi0;
    if (tmax < tmin)
        return false;
    if (!(lo2 <= tmin))
        tmin = lo2;
    if (!(hi2 <= tmax))
        tmax = hi2;
    if (tmax < tmin)
        return false;
    if (!(lo1 <= tmin))
        tmin = lo1;
    if (!(hi1 <= tmax))
        tmax = hi1;
    if (tmax < tmin)
        return false;
    g_collisionClosestParams[0] = tmin;
    return true;
}

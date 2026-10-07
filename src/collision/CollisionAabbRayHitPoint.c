// bdc 0x089e9e74 CollisionAabbRayHitPoint
#include "bdc.h"

/* If the ray (`CollisionRayShape`) hits the AABB (`CollisionBvhTestRay`), writes the entry
   point `origin + dir * t` to `out` (t = `g_collisionClosestParams[0]`; xyz, `out.w` is `origin.w`
   since `vadd.t` keeps lane 3 of the loaded origin). Returns the hit test result. */

bool CollisionAabbRayHitPoint(const float *aabb, const void *ray, ScePspFVector4 *out)
{
    const CollisionRayShape *r = ray;
    float t;
    float x;
    float y;
    float z;
    float w;
    bool hit;

    hit = CollisionBvhTestRay(aabb, ray);
    if (hit) {
        t = g_collisionClosestParams[0];
        x = r->dir.x * t;
        y = r->dir.y * t;
        z = r->dir.z * t;
        x = r->origin.x + x;
        y = r->origin.y + y;
        z = r->origin.z + z;
        w = r->origin.w;
        out->x = x;
        out->y = y;
        out->z = z;
        out->w = w;
    }
    return hit;
}

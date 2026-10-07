// bdc 0x089e9f00 CollisionAabbSegmentHitPoint
#include "bdc.h"

/* Segment version of `CollisionAabbRayHitPoint`: fills the static ray `g_collisionAabbSegmentRay`
   (type 1, set up once) from the segment (`origin` = `start`, `dir` = `dir` normalised and saturated
   to [-1, 1], `invDir` = per-axis reciprocal; a zero-length direction and zero axes give 0, both w
   lanes are 0), slab-tests it against the AABB with tmax = |seg.dir| (`CollisionAabbRaySlab`) and
   on a hit writes `start + ray.dir * g_collisionClosestParams[0]` to `out` (w = `start.w`). Returns
   the slab test result; `out` is untouched on a miss. */

bool CollisionAabbSegmentHitPoint(const float *aabb, const void *seg, ScePspFVector4 *out)
{
    const SegmentShape *s = (const SegmentShape *)seg;
    CollisionRayShape *ray = &g_collisionAabbSegmentRay;
    float lenSq;
    float len;
    float k;
    float t;
    float x;
    float y;
    float z;

    if (g_collisionAabbSegmentRayInit == 0) {
        g_collisionAabbSegmentRayInit = 1;
        ray->vtbl = g_collisionRayVtbl;
        ray->type = 1;
    }
    lenSq = s->dir[0] * s->dir[0] + s->dir[1] * s->dir[1] + s->dir[2] * s->dir[2];
    len = __builtin_sqrtf(lenSq);

    ray->origin.x = s->start[0];
    ray->origin.y = s->start[1];
    ray->origin.z = s->start[2];
    ray->origin.w = s->start[3];

    k = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        k = 0.0f;
    }
    ray->dir.x = VfSat1(s->dir[0] * k);
    ray->dir.y = VfSat1(s->dir[1] * k);
    ray->dir.z = VfSat1(s->dir[2] * k);
    ray->dir.w = 0.0f;

    x = VfRcp(ray->dir.x);
    if (ray->dir.x == 0.0f) {
        x = 0.0f;
    }
    y = VfRcp(ray->dir.y);
    if (ray->dir.y == 0.0f) {
        y = 0.0f;
    }
    z = VfRcp(ray->dir.z);
    if (ray->dir.z == 0.0f) {
        z = 0.0f;
    }
    ray->invDir.x = x;
    ray->invDir.y = y;
    ray->invDir.z = z;
    ray->invDir.w = 0.0f;

    if (!CollisionAabbRaySlab(len, aabb, ray)) {
        return false;
    }
    t = g_collisionClosestParams[0];
    x = ray->dir.x * t;
    y = ray->dir.y * t;
    z = ray->dir.z * t;
    out->x = s->start[0] + x;
    out->y = s->start[1] + y;
    out->z = s->start[2] + z;
    out->w = s->start[3];
    return true;
}

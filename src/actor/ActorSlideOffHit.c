// bdc 0x088dd08c ActorSlideOffHit
#include "bdc.h"

/* Collision response step of `ActorMoveWithCollision` for the sphere query `query`
   (`CollisionSphereQuery`), from the last hit (`g_collisionHitResult`). When the hit surface
   type (`g_collisionHitInfo` `surface`) is below 7 and the normal is shallow (|normal.y| <
   0.3f), the push is lengthened to `1 + 0.7f * atan2f(|normal.y|, |normal.xz|)` and `normal.y` is
   zeroed in the global (walls are treated as vertical); otherwise the push factor is 1. Places the
   sphere at `point + normal * (radius + 0.001f) * push` (w = the hit point's w), lowered by
   `radius` unless `keepY`. Replaces `delta` xyz by the surface tangent `(-normal.z, normal.y,
   normal.x)` scaled by `dot(delta, tangent) * 0.04f`, with Y cleared and w = 0 (the bank-zero lane
   S713 of `C710`). Adds it to that position and writes the result to `outPos`
   and the sphere centre (raised back by `radius` when `keepY` is clear), then calls the sphere's
   vtable entry 9 (update). */
void ActorSlideOffHit(CollisionSphereQuery *query, float *outPos, float *delta, s8 keepY)
{
    ScePspFVector4 *normal = &g_collisionHitResult.normal;
    ScePspFVector4 *point = &g_collisionHitResult.point;
    float push;
    float lenXZ;
    float reach;
    float sx, sy, sz;
    float px, py, pz, pw;
    float tx, ty, tz;
    float dot;
    float scale;
    const VtblEntry *update;

    push = 1.0f;
    if (g_collisionHitInfo.surface < 7) {
        if (fabsf(normal->y) < 0.3f) {
            lenXZ = __builtin_sqrtf(normal->x * normal->x + 0.0f * 0.0f + normal->z * normal->z);
            push = atan2f(fabsf(normal->y), lenXZ) * 0.7f + push;
            normal->y = 0.0f;
        }
    }
    reach = query->radius + 0.001f;
    sx = normal->x * reach;
    sy = normal->y * reach;
    sz = normal->z * reach;
    sx = sx * push;
    sy = sy * push;
    sz = sz * push;
    px = point->x + sx;
    py = point->y + sy;
    pz = point->z + sz;
    pw = point->w;
    if (keepY == 0) {
        py = py - query->radius;
    }

    /* tangent = (-normal.z, normal.y, normal.x) */
    tx = -normal->z;
    ty = normal->y;
    tz = normal->x;
    dot = delta[0] * tx + delta[1] * ty + delta[2] * tz;
    scale = dot * 0.04f;
    delta[0] = tx * scale;
    delta[1] = ty * scale;
    delta[2] = tz * scale;
    delta[3] = 0.0f;
    delta[1] = 0.0f;

    outPos[0] = px + delta[0];
    outPos[1] = py + delta[1];
    outPos[2] = pz + delta[2];
    outPos[3] = pw;
    query->center.x = outPos[0];
    query->center.y = outPos[1];
    query->center.z = outPos[2];
    query->center.w = outPos[3];
    if (keepY == 0) {
        query->center.y = query->center.y + query->radius;
    }
    update = &query->vtbl[9];
    ((void (*)(void *))update->fn)((u8 *)query + update->delta);
}

// bdc 0x0886097c BtlBakuganResolveSphereHit
#include "bdc.h"

/* Collision response step of `BtlBakuganMoveCollide` for the sphere query `sphere`: from the
   last hit (`g_collisionHitResult`) places the sphere at `point + normal * (radius + 0.1)`
   (w = the hit point's w), lowered by `radius` unless `keepHeight`. Replaces `velocity` by the
   surface tangent `(-normal.z, normal.y, normal.x)` scaled by `dot(velocity, tangent) * 0.04`,
   with y cleared and w = 0 (bank zero S713). Adds it to that position and writes the result
   (w = the position's w) to `outPos` and the sphere centre (raised back by `radius` when
   `keepHeight` is clear), then calls the sphere's vtable entry 9 (update). `self` is unused. */
void BtlBakuganResolveSphereHit(BtlBakugan *self, CollisionSphereQuery *sphere, float *outPos,
                                float *velocity, u8 keepHeight)
{
    const ScePspFVector4 *point = &g_collisionHitResult.point;
    const ScePspFVector4 *normal = &g_collisionHitResult.normal;
    float pos[4];
    float tangent[3];
    float reach;
    float scale;
    const VtblEntry *update;

    (void)self;
    reach = sphere->radius + 0.100000001f;
    pos[0] = point->x + normal->x * reach;
    pos[1] = point->y + normal->y * reach;
    pos[2] = point->z + normal->z * reach;
    pos[3] = point->w;
    if (keepHeight == 0) {
        pos[1] = pos[1] - sphere->radius;
    }

    tangent[0] = -normal->z;
    tangent[1] = normal->y;
    tangent[2] = normal->x;
    scale = (velocity[0] * tangent[0] + velocity[1] * tangent[1] + velocity[2] * tangent[2]) *
            0.0399999991f;

    velocity[0] = tangent[0] * scale;
    velocity[1] = tangent[1] * scale;
    velocity[2] = tangent[2] * scale;
    velocity[3] = 0.0f; /* w lane of C710: bank S713 */
    velocity[1] = 0.0f;

    outPos[0] = pos[0] + velocity[0];
    outPos[1] = pos[1] + velocity[1];
    outPos[2] = pos[2] + velocity[2];
    outPos[3] = pos[3];
    sphere->center.x = outPos[0];
    sphere->center.y = outPos[1];
    sphere->center.z = outPos[2];
    sphere->center.w = outPos[3];
    if (keepHeight == 0) {
        sphere->center.y = sphere->center.y + sphere->radius;
    }
    update = &sphere->vtbl[9];
    ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
}

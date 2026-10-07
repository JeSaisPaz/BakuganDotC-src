// bdc 0x08878044 BtlAttackSweepHit
#include "bdc.h"

/* Swept hit test of an attack. Loads the swept-sphere query `g_collisionSweptSphereDesc` with
   `start = from`, `dir = to` (whole vectors) and `radius`; on the attack's first two frames
   (`age < 2`) or with `fromOwner` set it shifts the sweep by the horizontal offset of `from` from the
   owner's position (`dir += d`, `start -= d`, d.y = 0). Fills the shared hit query record at
   `g_btlAttackHitPoint` (shape, owner, heading `atan2f(to.z, to.x)`, `hitKind`; the global
   attack flags take `flags`) and queries `CollisionHitQuery` on `mask` without the owner's own
   body layer: above radius 30 once on its layers 1..5 and once more with radius 30 on
   `0x1bf0300`. Without a collider hit it tries `CollisionRaycast` (`0x2fb72700`), giving the
   contact point lifted by 10, and then the arena bounds (`BtlStageGetArenaBounds`): a sweep whose
   start is outside them in x/z and whose `start.y + dir.y` is below the radius hits at the sweep's
   midpoint at height `radius`; both clear `g_btlAttackHitCollider`. A collider hit on a live unit
   (`BtlBakuganListFind`) queues the hit on it (`BtlBakuganQueueHit`). Returns non-zero on a
   hit, the hit point left in `g_btlAttackHitPoint`. */
u32 BtlAttackSweepHit(float radius, BtlAttack *self, float *from, float *to, s32 hitKind, s32 flags, char fromOwner, u32 mask)
{
    CollisionSweptSphereDesc *desc = &g_collisionSweptSphereDesc;
    CollisionCollider *body = self->owner->collider0;
    float offset[4];
    float half[3];
    float mid[4];
    int i;
    float *bounds;
    float len;
    u32 layerMask = mask & ~(1u << (body->layer & 0x1f));
    u32 hit;

    desc->start.x = from[0];
    desc->start.y = from[1];
    desc->start.z = from[2];
    desc->start.w = from[3];
    desc->dir.x = to[0];
    desc->dir.y = to[1];
    desc->dir.z = to[2];
    desc->dir.w = to[3];
    desc->radius = radius;
    if (self->age < 2 || fromOwner != 0) {
        /* offset = from - owner->pos (xyz, w from `from`), offset.y = 0 */
        for (i = 0; i < 4; i++) {
            offset[i] = from[i];
        }
        offset[0] = offset[0] - self->owner->base.pos[0];
        offset[1] = offset[1] - self->owner->base.pos[1];
        offset[2] = offset[2] - self->owner->base.pos[2];
        offset[1] = 0.0f;
        /* dir.xyz += offset; start.xyz -= offset */
        desc->dir.x = desc->dir.x + offset[0];
        desc->dir.y = desc->dir.y + offset[1];
        desc->dir.z = desc->dir.z + offset[2];
        desc->start.x = desc->start.x - offset[0];
        desc->start.y = desc->start.y - offset[1];
        desc->start.z = desc->start.z - offset[2];
    }
    desc->start.w = desc->radius * desc->radius;
    /* dir.w = |dir.xyz| */
    len = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y +
                          desc->dir.z * desc->dir.z);
    desc->dir.w = len;
    g_btlAttackHitShape = desc->shapeBlock;
    g_btlAttackHitOwner = self->owner;
    g_btlAttackHitHeading = atan2f(to[2], to[0]);
    g_btlAttackHitKind = hitKind;
    g_collisionAttackFlags = flags;

    if (!(radius <= 30.0f)) {
        char hitLarge = CollisionHitQuery(layerMask & 0x3e, &g_btlAttackHitPoint.x, 1, NULL);
        char hitSmall;

        desc->radius = 30.0f;
        hitSmall = CollisionHitQuery(0x1bf0300, &g_btlAttackHitPoint.x, 1, NULL);
        hit = (hitLarge | hitSmall) != 0;
    } else {
        hit = CollisionHitQuery(layerMask, &g_btlAttackHitPoint.x, 1, NULL);
    }

    if (hit != 0) {
        if (g_collisionLastHitCollider != NULL) {
            BtlBakugan *unit = g_collisionLastHitCollider->owner;

            if (BtlBakuganListFind(unit) != NULL) {
                BtlBakuganQueueHit(unit, (s16)self->type, self->owner);
            }
        }
        return hit;
    }

    if (CollisionRaycast(0x2fb72700, desc->shapeBlock, 0) != NULL) {
        hit = 1;
        /* hit point = contact point (whole vector) */
        g_btlAttackHitPoint = g_collisionHitResult.point;
        g_btlAttackHitPoint.y += 10.0f;
        g_btlAttackHitCollider = NULL;
        return hit;
    }

    bounds = BtlStageGetArenaBounds();
    if (!(bounds[0] <= desc->start.x) || bounds[3] < desc->start.x ||
        !(bounds[2] <= desc->start.z) || bounds[5] < desc->start.z) {
        if (desc->start.y + desc->dir.y < desc->radius) {
            hit = 1;
            /* hit point = start + dir * 0.5 (xyz; w from start) */
            half[0] = desc->dir.x * 0.5f;
            half[1] = desc->dir.y * 0.5f;
            half[2] = desc->dir.z * 0.5f;
            mid[0] = desc->start.x + half[0];
            mid[1] = desc->start.y + half[1];
            mid[2] = desc->start.z + half[2];
            mid[3] = desc->start.w;
            g_btlAttackHitPoint.x = mid[0];
            g_btlAttackHitPoint.y = mid[1];
            g_btlAttackHitPoint.z = mid[2];
            g_btlAttackHitPoint.w = mid[3];
            g_btlAttackHitPoint.y = desc->radius;
            g_btlAttackHitCollider = NULL;
        }
    }
    return hit;
}

// bdc 0x08878390 BtlAttackSweepHitSound
#include "bdc.h"

/* Beam variant of `BtlAttackSweepHit` used by `BtlAttackUpdateBeam`. Loads the swept-sphere query
   `g_collisionSweptSphereDesc` with `start = from`, `dir = to` (whole vectors) and half `radius`;
   on the attack's first frame (`age == 0`) or with `fromOwner` set it shifts the sweep by the
   horizontal offset of `from` from the owner's position (`dir += d`, `start -= d`, d.y = 0). Fills the
   shared hit query record at `g_btlAttackHitPoint` (shape, owner, heading `atan2f(to.z, to.x)`,
   `hitKind`, collider cleared; the global attack flags take `flags`). Below radius 30 a world
   `CollisionRaycast` (`0x2fb72500`) hit, and then a `CollisionHitQuery` hit on `0x1bf0300`, each
   count as a hit: the hit point becomes the contact point lifted by 10 and the sweep is shortened
   to end 50 past the contact. The full radius sweep then queries `mask` without the owner's own body
   layer (layers 1..5 only): a hit counts only if it found a collider (`g_btlAttackHitCollider`);
   a miss still queues a hit (`BtlBakuganQueueHit`) on the unit owning
   `g_collisionLastHitCollider` if that unit is live (`BtlBakuganListFind`). The hit point is
   restored to its value before that last query. From the second frame on a hit plays the attack's
   hit sound at the hit point (`BtlAttackPlaySound`). Returns 1 on a hit, else 0. */
int BtlAttackSweepHitSound(float radius, BtlAttack *self, float *from, float *to, s32 hitKind, s32 flags, char fromOwner, u32 mask)
{
    CollisionSweptSphereDesc *desc = &g_collisionSweptSphereDesc;
    u32 layerMask = mask & ~(1u << (self->owner->collider0->layer & 0x1f));
    ScePspFVector4 offset;
    ScePspFVector4 saved;
    float *ownerPos;
    float len;
    float lenSq;
    float scale;
    int hit;

    desc->start = *(ScePspFVector4 *)from;
    desc->dir = *(ScePspFVector4 *)to;
    desc->radius = radius * 0.5f;
    if (self->age == 0 || fromOwner != 0) {
        /* offset = from - owner->pos (xyz, w from `from`), then y cleared */
        ownerPos = self->owner->base.pos;
        offset = *(ScePspFVector4 *)from;
        offset.x = offset.x - ownerPos[0];
        offset.y = offset.y - ownerPos[1];
        offset.z = offset.z - ownerPos[2];
        offset.y = 0.0f;
        /* dir.xyz += offset; start.xyz -= offset */
        desc->dir.x = desc->dir.x + offset.x;
        desc->dir.y = desc->dir.y + offset.y;
        desc->dir.z = desc->dir.z + offset.z;
        desc->start.x = desc->start.x - offset.x;
        desc->start.y = desc->start.y - offset.y;
        desc->start.z = desc->start.z - offset.z;
    }
    desc->start.w = desc->radius * desc->radius;
    /* dir.w = |dir.xyz| */
    desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z);
    g_btlAttackHitShape = desc->shapeBlock;
    g_btlAttackHitOwner = self->owner;
    g_btlAttackHitHeading = atan2f(to[2], to[0]);
    g_btlAttackHitKind = hitKind;
    g_btlAttackHitCollider = NULL;
    hit = 0;
    g_collisionAttackFlags = flags;

    if (radius < 30.0f) {
        if (CollisionRaycast(0x2fb72500, desc->shapeBlock, 0) != NULL) {
            hit = 1;
            g_btlAttackHitPoint = g_collisionHitResult.point;
            g_btlAttackHitPoint.y += 10.0f;
            g_btlAttackHitCollider = NULL;
            /* len = |contact.xyz - from.xyz| + 50 */
            {
                float dx = g_collisionHitResult.point.x - from[0];
                float dy = g_collisionHitResult.point.y - from[1];
                float dz = g_collisionHitResult.point.z - from[2];

                len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
            }
            len = len + 50.0f;
            /* dir.xyz = normalize(dir.xyz) * len (inverse length 0 for a zero dir); dir.w = 0 */
            lenSq = desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z;
            scale = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
            scale = scale * len;
            desc->dir.x = desc->dir.x * scale;
            desc->dir.y = desc->dir.y * scale;
            desc->dir.z = desc->dir.z * scale;
            desc->dir.w = 0.0f;
            desc->start.w = desc->radius * desc->radius;
            desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z);
        }
    }

    if (CollisionHitQuery(0x1bf0300, &g_btlAttackHitPoint.x, 1, NULL) != 0) {
        hit = 1;
        g_btlAttackHitPoint = g_collisionHitResult.point;
        g_btlAttackHitPoint.y += 10.0f;
        g_btlAttackHitCollider = NULL;
        /* len = |contact.xyz - from.xyz| + 50 */
        {
            float dx = g_collisionHitResult.point.x - from[0];
            float dy = g_collisionHitResult.point.y - from[1];
            float dz = g_collisionHitResult.point.z - from[2];

            len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        }
        len = len + 50.0f;
        /* dir.xyz = normalize(dir.xyz) * len (inverse length 0 for a zero dir); dir.w = 0 */
        lenSq = desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z;
        scale = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        scale = scale * len;
        desc->dir.x = desc->dir.x * scale;
        desc->dir.y = desc->dir.y * scale;
        desc->dir.z = desc->dir.z * scale;
        desc->dir.w = 0.0f;
    }
    desc->radius = radius;
    desc->start.w = radius * radius;
    /* dir.w = |dir.xyz| */
    desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y + desc->dir.z * desc->dir.z);

    saved = g_btlAttackHitPoint;
    if (CollisionHitQuery(layerMask & 0x3e, &g_btlAttackHitPoint.x, 1, NULL) != 0) {
        if (g_btlAttackHitCollider != NULL) {
            hit = 1;
        }
    } else if (g_collisionLastHitCollider != NULL) {
        BtlBakugan *unit = g_collisionLastHitCollider->owner;

        if (BtlBakuganListFind(unit) != NULL) {
            BtlBakuganQueueHit(unit, (s16)self->type, self->owner);
        }
    }
    g_btlAttackHitPoint = saved;

    if (self->age != 0 && hit != 0) {
        BtlAttackPlaySound(self, self->params.hitSound, &g_btlAttackHitPoint.x, 0, 0);
    }
    return hit;
}

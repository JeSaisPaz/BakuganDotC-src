// bdc 0x0886ae40 BtlBakuganMotionHitCheck
#include "bdc.h"

/* Attack hit check for one motion hit event `ev` (`BtlMotionHitEvent`; the pending hit-window
   record or a motion event). Does nothing (returns 0) while `g_btlControlLockAll` is set.
   Otherwise sweeps `g_collisionSweptSphereDesc` from the unit's position raised by
   max(height / 2, `ev->size`) along its heading for `ev->reach` scaled by 1.2 (flag 4 with combo
   step < 2), 1.0 (state 0xb) or 0.8, with radius `ev->size` (attack ids 0x14/0x15: 40 lower, radius
   +25). The function-static query `g_btlBakuganHitQuery` carries the shape, the unit, its
   collider, heading, attack id (folded when `foldAttackIds`) and side (`id % 2 + 1`);
   `g_collisionAttackFlags` gets the event flags (3 when 0). State 0x14 tests only the target's
   collider (`CollisionTestPair`), state 0xb the target's collider exactly with layer bits 0x640
   masked out and only when the target is not locked by another attacker, other states all
   colliders (`CollisionHitQuery`). No hit in state 7 sets `attackResult` 4; a hit sets
   stateFlags 0x8000 (+0x2000 in state 0xb), `motionHitLanded`, and for attack ids 0x14/0x15 an
   input attack cooldown of 8. Returns the hit result. */

int BtlBakuganMotionHitCheck(BtlBakugan *self, void *ev)
{
    const BtlMotionHitEvent *hitEv = (const BtlMotionHitEvent *)ev;
    CollisionQuery *query = &g_btlBakuganHitQuery;
    CollisionSweptSphereDesc *desc = &g_collisionSweptSphereDesc;
    CollisionCollider *ownCollider;
    BtlBakugan *target;
    float height;
    float radius;
    float scale;
    float sweep;
    u32 layerMask;
    bool isState0b;
    s32 kind;
    int hit;

    if (g_btlBakuganHitQueryInited == 0) {
        g_btlBakuganHitQueryInited = 1;
        query->contact = g_gfxVecZero;
    }
    if (g_btlControlLockAll != 0) {
        return 0;
    }
    isState0b = self->state == 0xb;
    ownCollider = (CollisionCollider *)self->collider0;
    layerMask = (1u << (ownCollider->layer & 0x1f)) ^ 0x31bf337e;
    height = self->height * 0.5f;
    radius = hitEv->size;
    if (height < radius) {
        height = hitEv->size;
    }
    if (hitEv->attackId == 0x14 || hitEv->attackId == 0x15) {
        height = height + -40.0f;
        radius = radius + 25.0f;
    }
    if ((self->flags & 4) != 0 && self->comboStep < 2) {
        scale = 1.20000005f;
    } else if (isState0b) {
        scale = 1.0f;
    } else {
        scale = 0.800000012f;
    }
    desc->start.x = self->base.pos[0];
    desc->start.y = self->base.pos[1];
    desc->start.z = self->base.pos[2];
    desc->start.w = self->base.pos[3];
    desc->start.y = desc->start.y + height;
    sweep = hitEv->reach * scale;
    /* dir = (cos yaw, 0, sin yaw) * sweep; the vrot.q leaves lane w 0 */
    desc->dir.x = __builtin_cosf(self->base.rot[1]) * sweep;
    desc->dir.y = 0.0f * sweep;
    desc->dir.z = __builtin_sinf(self->base.rot[1]) * sweep;
    desc->dir.w = 0.0f;
    desc->radius = radius;
    desc->start.w = radius * radius;
    /* dir.w = |dir.xyz| */
    desc->dir.w = __builtin_sqrtf(desc->dir.x * desc->dir.x + desc->dir.y * desc->dir.y +
                                  desc->dir.z * desc->dir.z);

    query->shape = desc->shapeBlock;
    query->ownerCollider = self->collider0;
    query->owner = self;
    query->heading = self->base.rot[1];
    query->attackId = hitEv->attackId;
    query->attackSide = hitEv->attackId % 2 + 1;
    if (self->foldAttackIds != 0) {
        if (query->attackId >= 6 && query->attackId < 0xc) {
            query->attackId = query->attackId - 6;
        } else if (query->attackId >= 0x12 && query->attackId < 0x16) {
            kind = 0;
            if (((query->attackId - 0x12) & 1) != 0) {
                kind = 3;
            }
            query->attackId = kind;
        }
    }
    g_collisionAttackFlags = hitEv->flags != 0 ? hitEv->flags : 3;

    hit = 0;
    if (self->state == 0x14) {
        target = (BtlBakugan *)BtlBakuganGetTarget(self);
        if (target != NULL && target->collider0 != NULL) {
            hit = CollisionTestPair(layerMask, target->collider0, &query->contact.x, 0);
        }
    } else if (isState0b) {
        layerMask &= ~0x640u;
        target = (BtlBakugan *)BtlBakuganGetTarget(self);
        if (target != NULL && target->collider0 != NULL &&
            (target->lockedAttacker == NULL || target->lockedAttacker == self)) {
            hit = CollisionTestPair(layerMask, target->collider0, &query->contact.x, 1);
        }
    } else {
        hit = CollisionHitQuery(layerMask, &query->contact.x, 0, NULL);
    }
    if (hit == 0 && self->state == 7) {
        self->attackResult = 4;
    }
    if (hit != 0) {
        self->stateFlags = self->stateFlags | 0x8000;
        if (isState0b) {
            self->stateFlags = self->stateFlags | 0x2000;
        }
        self->motionHitLanded = 1;
        if (query->attackId == 0x14 || query->attackId == 0x15) {
            self->input->attackCooldown = 8;
        }
    }
    return hit;
}

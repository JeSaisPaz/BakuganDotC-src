// bdc 0x08864690 BtlBakuganTryBlock
#include "bdc.h"

/* Block virtual of the battle Bakugan, queried by BtlBakuganOnHit for a guardable hit while the
   unit is guarding. Returns 0 when the unit has no body collider, else 1. The attacker is the body
   collider's hitAttacker for hit kinds 1..3 (kinds 2/3 come from an attack object). The block
   direction is the attacker's position (or the contact point when there is none) minus
   guardAnchor, flattened (y = 0) and normalised (0 vector stays 0: bank S713 replaces a zero
   length's reciprocal). Early guard (guardAge < 5): sets state flag
   0x80; kind 1 spawns guard effect 0x3d at the anchor owned by this unit, hits back
   (BtlBakuganInflictCounterHit, strong), halves the collider's hit duration and rescales the
   velocity to length 5; kind 2 reflects the attacker's pending attack (BtlAttackFindPendingByOwner,
   BtlAttackReflect); then counts stat 10, plays sound 0x200071 and enters state 0x12. Late guard:
   the collider's hit duration becomes the species' guardHitDuration, effect 0x38 spawns at the
   anchor, motion 0x103 plays and stat 0xf is counted. Both set blockTimer to 15 and play sound
   0x200070 (attack hit) or 0x20006f. */

int BtlBakuganTryBlock(BtlBakugan *self)
{
    float dir[4];
    float lenSq;
    float k;
    CollisionCollider *body;
    BtlBakugan *owner = NULL;
    BtlAttack *attack;
    GfxEffect *effect;
    const float *from;
    float *anchor;
    bool fromAttack = false;
    s32 kind;

    if (self->collider0 == NULL) {
        return 0;
    }
    body = self->collider0;
    kind = body->hitParam164;
    anchor = self->guardAnchor;
    if (kind < 2) {
        if (kind > 0) {
            owner = self->collider0->hitAttacker;
        }
    } else if (kind < 4) {
        owner = self->collider0->hitAttacker;
        fromAttack = true;
    }
    if (owner != NULL) {
        from = owner->base.pos;
    } else {
        from = &self->collider0->hitPos.x;
    }
    /* dir = from - anchor (xyz), flattened (y = 0) */
    dir[0] = from[0] - anchor[0];
    dir[2] = from[2] - anchor[2];
    dir[1] = 0.0f;
    /* dir.xyz = clamp(normalise(dir.xyz), -1, 1), scale 0 for a zero length (bank S713); w = S713 = 0 */
    lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    dir[0] = VfSat1(dir[0] * k);
    dir[1] = VfSat1(dir[1] * k);
    dir[2] = VfSat1(dir[2] * k);
    dir[3] = 0.0f;
    if (self->guardAge < 5) {
        self->stateFlags |= 0x80;
        body = self->collider0;
        if (body->hitParam164 == 1) {
            effect = GfxEffectSpawnAttachedDir(g_worldEffectMgr, 0x3d, anchor, dir);
            effect->ownerBakugan = self;
            if (self != NULL) {
                effect->ownerId = self->base.base.id;
            }
            BtlBakuganInflictCounterHit(self, owner, 1);
            body = self->collider0;
            body->hitDuration = body->hitDuration / 2;
            /* velocity.xyz = normalise(velocity.xyz) * 5 (zero length: 0), w = 0 */
            lenSq = self->base.velocity[0] * self->base.velocity[0] +
                    self->base.velocity[1] * self->base.velocity[1] +
                    self->base.velocity[2] * self->base.velocity[2];
            k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
            k = k * 5.0f;
            self->base.velocity[0] = self->base.velocity[0] * k;
            self->base.velocity[1] = self->base.velocity[1] * k;
            self->base.velocity[2] = self->base.velocity[2] * k;
            self->base.velocity[3] = 0.0f;
        } else if (self->collider0->hitParam164 == 2) {
            attack = BtlAttackFindPendingByOwner(owner);
            if (attack != NULL) {
                attack->reflector = self;
                BtlAttackReflect(attack);
            }
        }
        if (self->stats != NULL) {
            BtlStatsAddCounter(self->stats, 10, 1);
        }
        BtlBakuganPlaySound(self, 0x200071, 0, 0);
        BtlBakuganSetState(self, 0x12, 0);
    } else {
        self->collider0->hitDuration = self->combat.stats->guardHitDuration;
        GfxEffectSpawnAttachedDir(g_worldEffectMgr, 0x38, anchor, dir);
        BtlBakuganPlayMotion(0.0f, self, 0x103, 1, 1);
        if (self->stats != NULL) {
            BtlStatsAddCounter(self->stats, 0xf, 1);
        }
    }
    self->blockTimer = 15;
    if (fromAttack) {
        BtlBakuganPlaySound(self, 0x200070, 0, 0);
    } else {
        BtlBakuganPlaySound(self, 0x20006f, 0, 0);
    }
    return 1;
}

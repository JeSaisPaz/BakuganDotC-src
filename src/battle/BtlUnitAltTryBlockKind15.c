// bdc 0x0885c570 BtlUnitAltTryBlockKind15
#include "bdc.h"

/* Block reaction of the kind-0x15 unit (`BtlUnitAlt` slot 25 via `BtlUnitAltTryBlock`). Returns 0
   without a body collider (`collider0`). The attacker is the collider's `hitAttacker` for hit
   kinds (`hitParam164`) 1..3 (kinds 2/3 mark the hit as guardable). It computes the direction from
   `guardAnchor` to the attacker's position (or the contact point `hitPos` when there is none),
   flattened (y = 0) and normalised in the VFPU (zero length scales by the bank's 0, S713); the result is
   left on the stack and unused. Only a guardable hit whose kind is exactly 2 is blocked: counts
   stat 0xf (`BtlStatsAddCounter`, when the unit has stats), plays sound 0x1800002
   (`BtlBakuganPlaySound`), spawns effect 0x5f at the contact point on `g_btlUnitEffectMgr`
   (`GfxEffectSpawn`), sets `blockTimer` to 25 and state flag 0x80, sets the collider's hit-active
   flag and a 30-frame `hitTimer`, enters state 0x14 (`BtlBakuganSetState`), zeroes the velocity (bank C720) and returns 1; otherwise returns 0. */
int BtlUnitAltTryBlockKind15(BtlBakugan *self)
{
    float dir[4] __attribute__((aligned(16)));
    CollisionCollider *body;
    const BtlBakugan *attacker = NULL;
    const float *from;
    bool guardable = false;
    s32 kind;

    if (self->collider0 == NULL) {
        return 0;
    }
    kind = ((CollisionCollider *)self->collider0)->hitParam164;
    if (kind < 2) {
        if (kind > 0) {
            attacker = ((CollisionCollider *)self->collider0)->hitAttacker;
        }
    } else if (kind < 4) {
        guardable = true;
        attacker = ((CollisionCollider *)self->collider0)->hitAttacker;
    }
    if (attacker != NULL) {
        from = attacker->base.pos;
    } else {
        from = &((CollisionCollider *)self->collider0)->hitPos.x;
    }
    /* dir = from - guardAnchor (xyz; w copied), flattened, normalised (scale 0 for zero length,
       bank S713) and clamped to [-1, 1]; left on the stack, unused */
    {
        float lenSq;
        float k;

        dir[0] = from[0] - self->guardAnchor[0];
        dir[1] = 0.0f;
        dir[2] = from[2] - self->guardAnchor[2];
        dir[3] = from[3];
        lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        dir[0] = VfSat1(dir[0] * k);
        dir[1] = VfSat1(dir[1] * k);
        dir[2] = VfSat1(dir[2] * k);
    }
    if (!guardable || ((CollisionCollider *)self->collider0)->hitParam164 != 2) {
        return 0;
    }
    if (self->stats != NULL) {
        BtlStatsAddCounter(self->stats, 0xf, 1);
    }
    BtlBakuganPlaySound(self, 0x1800002, 0, 0);
    GfxEffectSpawn(g_btlUnitEffectMgr, 0x5f, &((CollisionCollider *)self->collider0)->hitPos.x);
    self->blockTimer = 25;
    self->stateFlags |= 0x80;
    body = self->collider0;
    body->flags |= 1;
    body->hitTimer = 30;
    BtlBakuganSetState(self, 0x14, 0);
    /* velocity = C720, the bank's zero vector */
    self->base.velocity[0] = 0.0f;
    self->base.velocity[1] = 0.0f;
    self->base.velocity[2] = 0.0f;
    self->base.velocity[3] = 0.0f;
    return 1;
}

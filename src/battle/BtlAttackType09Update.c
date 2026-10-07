// bdc 0x0887bac4 BtlAttackType09Update
#include "bdc.h"

/* Per-frame handler of attack type 9 (entry 9 of the attack handler table run by
   `BtlAttackUpdate`): a straight shot. After 60 frames of age it ends (`BtlAttackEnd`). On frame
   0 it scales `vel` by 50 (x, y, z; the asm also stores a stale w lane, left out).
   On later frames it runs the clash test (`BtlAttackCheckClash`, result ignored), sweeps
   the hit test along `vel` (hit kind 0x2c, arg 3, mask `0x31bf337e`, `BtlAttackSweepHit`) and on
   a hit spawns impact effect 0x7e at `g_btlAttackHitPoint` owned by the attack's owner Bakugan
   and ends; otherwise the position advances by `vel` (x, y, z; w kept). */

void BtlAttackType09Update(BtlAttack *self)
{
    GfxEffect *effect;
    BtlBakugan *owner;
    float *pos;

    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        /* vel.xyz *= 50.0f; the w lane of the sv.q is stale (C710 lane 3 never set here) */
        self->vel[0] *= 50.0f;
        self->vel[1] *= 50.0f;
        self->vel[2] *= 50.0f;
        return;
    }
    BtlAttackCheckClash(self);
    pos = self->pos;
    if (BtlAttackSweepHit(self->radius, self, pos, self->vel, 0x2c, 3, 0, 0x31bf337e) != 0) {
        effect = (GfxEffect *)GfxEffectSpawn(g_btlAttackEffectMgr, 0x7e, &g_btlAttackHitPoint.x);
        owner = self->owner;
        effect->ownerBakugan = owner;
        if (owner != NULL) {
            effect->ownerId = owner->base.base.id;
        }
        BtlAttackEnd(self);
        return;
    }
    /* pos.xyz += vel.xyz (w kept) */
    pos[0] += self->vel[0];
    pos[1] += self->vel[1];
    pos[2] += self->vel[2];
}

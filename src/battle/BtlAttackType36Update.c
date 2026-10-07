// bdc 0x0887f284 BtlAttackType36Update
#include "bdc.h"

/* Per-frame handler of attack type 0x36 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   short horizontal thrust: on the first frame flattens its direction (y = 0, normalised) and spawns
   the directed effect 0xe0 tagged with the owner Bakugan; for frames 0..16 it follows that effect's
   position, sets the velocity to `g_vecUp` and, from frame 1, sweeps from the effect position
   along the direction (radius 25 up to frame 5, then 100; hit kind 0x59, arg 3); after frame 16
   it ends. The normalised direction is clamped per lane
   to [-1, 1], 0 for a zero-length vector, with w = 0. */

void BtlAttackType36Update(BtlAttack *self)
{
    GfxEffect *effect;
    BtlBakugan *owner;

    if (self->age > 16) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->dir[1] = 0.0f;
        /* dir = normalise(dir.xyz), each lane clamped to [-1, 1], 0 for zero length; w becomes
           the bank zero S713. */
        {
            float lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1] +
                          self->dir[2] * self->dir[2];
            float k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
            self->dir[0] = VfSat1(self->dir[0] * k);
            self->dir[1] = VfSat1(self->dir[1] * k);
            self->dir[2] = VfSat1(self->dir[2] * k);
            self->dir[3] = 0.0f;
        }
        effect = GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xe0, self->pos, self->dir);
        self->effect = effect;
        owner = self->owner;
        effect->ownerBakugan = owner;
        if (owner != NULL) {
            effect->ownerId = owner->base.base.id;
        }
    }
    effect = (GfxEffect *)self->effect;
    self->pos[0] = effect->pos[0];
    self->pos[1] = effect->pos[1];
    self->pos[2] = effect->pos[2];
    self->pos[3] = effect->pos[3];
    self->vel[0] = g_vecUp.x;
    self->vel[1] = g_vecUp.y;
    self->vel[2] = g_vecUp.z;
    self->vel[3] = g_vecUp.w;
    if (self->age < 6) {
        self->radius = 25.0f;
    } else {
        self->radius = 100.0f;
    }
    if (self->age > 0) {
        BtlAttackSweepHit(self->radius, self, ((GfxEffect *)self->effect)->pos, self->dir, 0x59, 3, 0,
                          0x31bf337e);
    }
}

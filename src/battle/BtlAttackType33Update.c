// bdc 0x0887ee90 BtlAttackType33Update
#include "bdc.h"

/* Per-frame handler of attack type 0x33 (handler table `0x08a685f0`, run by `BtlAttackUpdate`).
   Ends (`BtlAttackEnd`) once `age` (as float) is past 150. On the first frame raises `pos.y` by
   30, moves `pos.xyz` 65 units along `dir`, aims at the target with `turnRate` 1
   (`BtlAttackSteerToTarget` speed 0, height 40) and then sets `turnRate` 0.02. Later, unless
   `BtlAttackResolveClash` (impact 0x1b) consumed it, homes at speed 30 / height 130 and sweeps
   for hits (kind 0x56, flags 3, `BtlAttackSweepHit`): on a hit spawns impact 0x1b at `pos`
   when no collider was hit or its `layer` is 9 (`g_btlAttackHitCollider`), plays sound
   0x200097 (`BtlAttackPlaySound`) and ends; otherwise copies `dir` to the effect's `dir` and
   advances `pos` by `vel`. */
void BtlAttackType33Update(BtlAttack *self)
{
    CollisionCollider *collider;
    GfxEffect *effect;

    if (!((float)self->age <= 150.0f)) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->pos[1] = self->pos[1] + 30.0f;
        /* pos.xyz += dir.xyz * 65 */
        self->pos[0] = self->pos[0] + self->dir[0] * 65.0f;
        self->pos[1] = self->pos[1] + self->dir[1] * 65.0f;
        self->pos[2] = self->pos[2] + self->dir[2] * 65.0f;
        self->turnRate = 1.0f;
        BtlAttackSteerToTarget(0.0f, 40.0f, self, 1, NULL);
        self->turnRate = 0.0199999996f;
        return;
    }
    if (BtlAttackResolveClash(self, 0x1b) != 0) {
        return;
    }
    BtlAttackSteerToTarget(30.0f, 130.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x56, 3, 0, 0x31bf337e) != 0) {
        collider = (CollisionCollider *)g_btlAttackHitCollider;
        if (collider == NULL || collider->layer == 9) {
            GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, self->pos);
        }
        BtlAttackPlaySound(self, 0x200097, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    effect = (GfxEffect *)self->effect;
    effect->dir[0] = self->dir[0];
    effect->dir[1] = self->dir[1];
    effect->dir[2] = self->dir[2];
    effect->dir[3] = self->dir[3];
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}

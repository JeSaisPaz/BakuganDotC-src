// bdc 0x0887f080 BtlAttackType34Update
#include "bdc.h"

/* Per-frame handler of attack type 0x34 (entry 52 of the attack handler table run by
   `BtlAttackUpdate`). After 90 frames of age it stops its attached effects 0x23f/0x240 and ends.
   On frame 0 it lifts the position by 35 in y, moves it 65 units along `dir` and turns fully
   towards the target (`BtlAttackSteerToTarget` speed 0, height 40, turn rate 1), then sets the
   turn rate to 0.02; from frame 20 on it homes with speed 35 and height 100. Every frame after
   frame 0 it tests clashes and sweeps the hit test along `vel`; on a hit it spawns effect 0x1b at
   the hit point, stops the attached effects, plays sound 0x200098 and ends, otherwise the attached
   effect takes `dir` and the position advances by `vel`. */
void BtlAttackType34Update(BtlAttack *self)
{
    float step[3];
    GfxEffect *effect;

    if (self->age > 90) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x23f, self->pos);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x240, self->pos);
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->pos[1] = self->pos[1] + 35.0f;
        /* pos.xyz += dir.xyz * 65 (pos.w kept) */
        step[0] = self->dir[0] * 65.0f;
        step[1] = self->dir[1] * 65.0f;
        step[2] = self->dir[2] * 65.0f;
        self->pos[0] = self->pos[0] + step[0];
        self->pos[1] = self->pos[1] + step[1];
        self->pos[2] = self->pos[2] + step[2];
        self->turnRate = 1.0f;
        BtlAttackSteerToTarget(0.0f, 40.0f, self, 1, NULL);
        self->turnRate = 0.0199999996f;
        return;
    }
    if (self->age >= 20) {
        BtlAttackSteerToTarget(35.0f, 100.0f, self, 1, NULL);
    }
    BtlAttackCheckClash(self);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x57, 3, 0, 0x31bf337e) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, &g_btlAttackHitPoint.x);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x23f, self->pos);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x240, self->pos);
        BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    /* effect->dir = dir (all four lanes); pos.xyz += vel.xyz (w kept) */
    effect = (GfxEffect *)self->effect;
    effect->dir[0] = self->dir[0];
    effect->dir[1] = self->dir[1];
    effect->dir[2] = self->dir[2];
    effect->dir[3] = self->dir[3];
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}

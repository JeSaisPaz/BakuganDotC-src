// bdc 0x0887bd90 BtlAttackType0DUpdate
#include "bdc.h"

/* Burst and end (inlined twice in the binary): spawns effect 0x1b at the attack position, plays
   sound 0x200097 and ends the attack. */
static inline void BtlAttackType0DBurst(BtlAttack *self)
{
    GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, self->pos);
    BtlAttackPlaySound(self, 0x200097, NULL, 0, 0);
    BtlAttackEnd(self);
}

/* Per-frame handler of attack type 0xd (handler table `0x08a685f0`, run by `BtlAttackUpdate`).
   After 60 frames it sets state 2 on its attached effects 0x98 and 0x1ce
   (`GfxEffectSetStateAttached`), clears `effect` and ends. Otherwise `paramF0` eases 20% of the
   way towards 60 each frame; above 50 it is capped at 50 and `phase` is incremented (the attack
   starts moving); the value is written to the attached effect's `size[1]`. Frame 0 copies the
   velocity scaled by -200 into `auxVec` and scales the velocity by 50 (xyz; the w lanes become
   0, VFPU bank S713). Later frames: on cancellation or a clash (`BtlAttackCheckClash`)
   it bursts (effect 0x1b at the position, sound 0x200097, end); otherwise it sweeps from
   `pos + auxVec` to `vel - auxVec` (xyz; `BtlAttackSweepHit`, hit kind 0x30, arg 1). A hit with
   no world collider (`g_btlAttackHitCollider`) bursts; a collider hit spawns effect 0x1b at
   `g_btlAttackHitPoint` on even `param0` counts, increments `param0` and plays the sound every
   fourth count. Finally, once `phase` is non-zero, `pos.xyz += vel.xyz`. */
void BtlAttackType0DUpdate(BtlAttack *self)
{
    float to[4] __attribute__((aligned(16)));
    float from[4] __attribute__((aligned(16)));
    float size;

    if (self->age > 60) {
        GfxEffectSetStateAttached(g_btlAttackEffectMgr, 0x98, self->pos, 2);
        GfxEffectSetStateAttached(g_btlAttackEffectMgr, 0x1ce, self->pos, 2);
        self->effect = NULL;
        BtlAttackEnd(self);
        return;
    }
    size = self->paramF0 + (60.0f - self->paramF0) * 0.200000003f;
    self->paramF0 = size;
    if (!(size <= 50.0f)) {
        self->paramF0 = 50.0f;
        self->phase++;
    }
    ((GfxEffect *)self->effect)->size[1] = self->paramF0;
    if (self->age == 0) {
        /* vscl.t writes C710 and sv.q stores its w lane: bank S713 = 0 */
        self->auxVec[0] = self->vel[0] * -200.0f;
        self->auxVec[1] = self->vel[1] * -200.0f;
        self->auxVec[2] = self->vel[2] * -200.0f;
        self->auxVec[3] = 0.0f;
        self->vel[0] = self->vel[0] * 50.0f;
        self->vel[1] = self->vel[1] * 50.0f;
        self->vel[2] = self->vel[2] * 50.0f;
        self->vel[3] = 0.0f;
        return;
    }
    if (self->cancelled != 0 || BtlAttackCheckClash(self) != 0) {
        BtlAttackType0DBurst(self);
        return;
    }
    /* to = -auxVec + vel (xyz; w = auxVec.w), from = pos + auxVec (xyz; w = pos.w) */
    to[0] = -self->auxVec[0] + self->vel[0];
    to[1] = -self->auxVec[1] + self->vel[1];
    to[2] = -self->auxVec[2] + self->vel[2];
    to[3] = self->auxVec[3];
    from[0] = self->pos[0] + self->auxVec[0];
    from[1] = self->pos[1] + self->auxVec[1];
    from[2] = self->pos[2] + self->auxVec[2];
    from[3] = self->pos[3];
    if (BtlAttackSweepHit(self->radius, self, from, to, 0x30, 1, 0, 0x31bf337e) != 0) {
        if (g_btlAttackHitCollider == NULL) {
            BtlAttackType0DBurst(self);
            return;
        }
        if ((self->param0 & 1) == 0) {
            GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, &g_btlAttackHitPoint.x);
        }
        self->param0++;
        if ((self->param0 & 3) == 0) {
            BtlAttackPlaySound(self, 0x200097, NULL, 0, 0);
        }
    }
    if (self->phase != 0) {
        self->pos[0] = self->pos[0] + self->vel[0];
        self->pos[1] = self->pos[1] + self->vel[1];
        self->pos[2] = self->pos[2] + self->vel[2];
    }
}

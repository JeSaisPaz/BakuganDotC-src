// bdc 0x0887db54 BtlAttackUpdateDecelHomingOrb
#include "bdc.h"

/* Burst and end (inlined three times in the binary): stops the attack's effects of id 0x200
   attached to `pos`, spawns `burstEffect` there bound to the owner, plays sound 0x200098 at the
   attack position and ends the attack. */
static inline void BtlAttackDecelOrbBurst(BtlAttack *self, int burstEffect)
{
    GfxEffect *burst;
    BtlBakugan *owner;

    GfxEffectStopAttached(g_btlAttackEffectMgr, 0x200, self->pos);
    burst = GfxEffectSpawn(g_btlAttackEffectMgr, burstEffect, self->pos);
    owner = self->owner;
    burst->ownerBakugan = owner;
    if (owner != NULL) {
        burst->ownerId = owner->base.base.id;
    }
    BtlAttackPlaySound(self, 0x200098, NULL, 0, 0);
    BtlAttackEnd(self);
}

/* Shared update of a slowing homing orb (attack types 0x1e and 0x66, burst effect 0xbd). The
   speed lives in `mtx[0][0]` and the upward kick in `paramF2`. Frame 0 sets speed 30 and kick 8.
   Later frames home on the target with a 120-unit height offset (`BtlAttackSteerToTarget`,
   decaying turn); from frame 5 sweep for hits (`BtlAttackSweepHit`, kind `hitId`, arg 3, half
   radius and flag set on frame 5 only); then the speed drops by 0.3 (to 0 unless the result's bit
   pattern is a positive integer, i.e. a positive float), the position moves by the velocity
   (xyz) plus the kick in Y and the kick decays x0.8. After 120 frames, on cancellation, a
   clash (`BtlAttackCheckClash`) or a hit, it bursts: stops its effects 0x200, spawns
   `burstEffect` bound to the owner, plays sound 0x200098 and ends (`BtlAttackEnd`). */
void BtlAttackUpdateDecelHomingOrb(BtlAttack *self, s32 hitId, int burstEffect)
{
    float radius;
    bool firstSweep;
    union { float f; s32 i; } speed;

    if (self->age > 120) {
        BtlAttackDecelOrbBurst(self, burstEffect);
        return;
    }
    if (self->age == 0) {
        self->mtx[0][0] = 30.0f;
        self->paramF2 = 8.0f;
        return;
    }
    BtlAttackSteerToTarget(self->mtx[0][0], 120.0f, self, 1, NULL);
    if (self->cancelled != 0 || BtlAttackCheckClash(self) != 0) {
        BtlAttackDecelOrbBurst(self, burstEffect);
        return;
    }
    if (self->age >= 5) {
        radius = self->radius;
        firstSweep = self->age == 5;
        if (firstSweep) {
            radius = radius * 0.5f;
        }
        if (BtlAttackSweepHit(radius, self, self->pos, self->vel, hitId, 3, firstSweep,
                              0x31bf337e) != 0) {
            BtlAttackDecelOrbBurst(self, burstEffect);
            return;
        }
    }
    /* mfc1 + bgtz: the clamp tests the float's bits as a signed integer */
    speed.f = self->mtx[0][0] - 0.300000012f;
    if (!(speed.i > 0)) {
        speed.f = 0.0f;
    }
    self->mtx[0][0] = speed.f;
    /* pos.xyz += vel.xyz (vadd.t; w keeps pos.w) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
    self->pos[1] = self->pos[1] + self->paramF2;
    self->paramF2 = self->paramF2 * 0.800000012f;
}

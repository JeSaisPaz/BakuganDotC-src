// bdc 0x088794ec BtlAttackUpdateSeekerBolt
#include "bdc.h"

/* Shared update of a short-lived homing bolt (attack types 5 and 0x78). Once age > 30 the attack
   ends. On frame 0 the position moves 80 times the velocity (xyz) and 30 up. Afterwards it steers
   at speed 200 with a 60-unit height offset (BtlAttackSteerToTarget); when cancelled or clashing
   (BtlAttackCheckClash) it spawns effect 0x1b at its position and ends; when the sweep from pos
   along vel hits (BtlAttackSweepHit with radius, `hitKind`, hit kind 3) it plays sound 0x200097,
   spawns effect 0x1b at the hit point and ends. Otherwise it spawns ten 0x97 trail effects from
   pos in steps of 0.1 x vel and advances pos by vel (xyz). Every ending also stops the seeker
   effects anchored at pos (BtlAttackStopSeekerEffects). */

void BtlAttackUpdateSeekerBolt(BtlAttack *self, s32 hitKind)
{
    float trail[4] __attribute__((aligned(16)));
    float step[4] __attribute__((aligned(16)));
    float *pos;
    float *vel;
    s32 i;

    pos = self->pos;
    if (self->age > 30) {
        BtlAttackEnd(self);
        BtlAttackStopSeekerEffects(g_btlAttackEffectMgr, pos);
        return;
    }
    if (self->age == 0) {
        /* pos.xyz += vel.xyz * 80 (pos.w kept) */
        step[0] = self->vel[0] * 80.0f;
        step[1] = self->vel[1] * 80.0f;
        step[2] = self->vel[2] * 80.0f;
        pos[0] = pos[0] + step[0];
        pos[1] = pos[1] + step[1];
        pos[2] = pos[2] + step[2];
        self->pos[1] = self->pos[1] + 30.0f;
        return;
    }
    BtlAttackSteerToTarget(200.0f, 60.0f, self, 1, NULL);
    if (self->cancelled != 0 || BtlAttackCheckClash(self) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, pos);
        BtlAttackEnd(self);
        BtlAttackStopSeekerEffects(g_btlAttackEffectMgr, pos);
        return;
    }
    vel = self->vel;
    if (BtlAttackSweepHit(self->radius, self, pos, vel, hitKind, 3, 0, 0x31bf337e) != 0) {
        BtlAttackPlaySound(self, 0x200097, NULL, 0, 0);
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x1b, &g_btlAttackHitPoint.x);
        BtlAttackEnd(self);
        BtlAttackStopSeekerEffects(g_btlAttackEffectMgr, pos);
        return;
    }
    /* trail = pos (all four lanes), step.xyz = vel.xyz * 0.1; step[3] would hold a stale VFPU
       lane and is never read */
    trail[0] = pos[0];
    trail[1] = pos[1];
    trail[2] = pos[2];
    trail[3] = pos[3];
    step[0] = vel[0] * 0.100000001f;
    step[1] = vel[1] * 0.100000001f;
    step[2] = vel[2] * 0.100000001f;
    for (i = 0; i < 10; i++) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0x97, trail);
        trail[0] = trail[0] + step[0];
        trail[1] = trail[1] + step[1];
        trail[2] = trail[2] + step[2];
    }
    /* pos.xyz += vel.xyz */
    pos[0] = pos[0] + vel[0];
    pos[1] = pos[1] + vel[1];
    pos[2] = pos[2] + vel[2];
}

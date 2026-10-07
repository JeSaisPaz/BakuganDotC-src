// bdc 0x08881d04 BtlAttackType6DUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x6d (entry 109 of the handler table `0x08a685f0` run by
   `BtlAttackUpdate`). Ground-hugging projectile: once its age exceeds 90 frames it ends. On
   frames after 0 it measures the pitch of `dir` (atan2f of y over the horizontal length); above
   0.5235988 rad (30 degrees) it rotates `dir` by the difference with the quaternion
   q = (axis * sin(delta/2), cos(delta/2)), axis = (-z, y, x) of dir: dir = q * (dir.xyz, 0) * conj(q)
   (all four lanes of the product stored). Then it homes (`BtlAttackSteerToTarget` speed 45,
   height 80). When it is flagged cancelled or `BtlAttackCheckClash` cancels it, it spawns effect
   0xc5 at its position and ends. Otherwise it sweeps the hit test along `vel` (hit kind 0x90); on a
   hit it spawns 0xc5, plays sound 0x200097 and ends; else the attached effect takes `dir`, the
   position advances by `vel`, and when the ground ray (`CollisionRaycastPoint`) hits less than
   200 below the position it spawns the directed trail effect 0xfd at the ground point. */

void BtlAttackType6DUpdate(BtlAttack *self)
{
    float ground[4];
    float q[4];
    float v[4];
    float t[4];
    float pitch;
    float delta;
    float half;
    float s;
    float *pos;
    float *edir;

    if (!((float)self->age <= 90.0f)) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        return;
    }
    pitch = atan2f(self->dir[1],
                   __builtin_sqrtf(self->dir[0] * self->dir[0] + self->dir[2] * self->dir[2]));
    pos = self->pos;
    if (!(pitch <= 0.523598790f)) {
        delta = 0.523598790f - pitch;
        /* VFPU sin/cos take quarter turns: delta * 1/pi gives the half angle */
        half = 0.318309873f * delta;
        s = VfSinQuarter(half);
        q[0] = -self->dir[2] * s;
        q[1] = self->dir[1] * s;
        q[2] = self->dir[0] * s;
        q[3] = VfCosQuarter(half);
        /* v = dir with w = 0 (bank S730) */
        v[0] = self->dir[0];
        v[1] = self->dir[1];
        v[2] = self->dir[2];
        v[3] = 0.0f;
        /* t = q * v */
        t[0] = q[0] * v[3] + q[1] * v[2] - q[2] * v[1] + q[3] * v[0];
        t[1] = -q[0] * v[2] + q[1] * v[3] + q[2] * v[0] + q[3] * v[1];
        t[2] = q[0] * v[1] - q[1] * v[0] + q[2] * v[3] + q[3] * v[2];
        t[3] = -q[0] * v[0] - q[1] * v[1] - q[2] * v[2] + q[3] * v[3];
        /* dir = t * conj(q), conj(q) = (-q.xyz, q.w) */
        q[0] = -q[0];
        q[1] = -q[1];
        q[2] = -q[2];
        self->dir[0] = t[0] * q[3] + t[1] * q[2] - t[2] * q[1] + t[3] * q[0];
        self->dir[1] = -t[0] * q[2] + t[1] * q[3] + t[2] * q[0] + t[3] * q[1];
        self->dir[2] = t[0] * q[1] - t[1] * q[0] + t[2] * q[3] + t[3] * q[2];
        self->dir[3] = -t[0] * q[0] - t[1] * q[1] - t[2] * q[2] + t[3] * q[3];
    }
    BtlAttackSteerToTarget(45.0f, 80.0f, self, 1, NULL);
    if (self->cancelled != 0 || BtlAttackCheckClash(self) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0xc5, pos);
        BtlAttackEnd(self);
        return;
    }
    if (BtlAttackSweepHit(self->radius, self, pos, self->vel, 0x90, 3, 0, 0x31bf337e) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0xc5, pos);
        BtlAttackPlaySound(self, 0x200097, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    /* the attached effect's direction takes all four lanes of dir; pos.xyz += vel.xyz (w kept) */
    edir = ((GfxEffect *)self->effect)->dir;
    edir[0] = self->dir[0];
    edir[1] = self->dir[1];
    edir[2] = self->dir[2];
    edir[3] = self->dir[3];
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
    if (CollisionRaycastPoint(pos, ground) != 0 && self->pos[1] - 200.0f < ground[1]) {
        GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xfd, ground, self->dir);
    }
}

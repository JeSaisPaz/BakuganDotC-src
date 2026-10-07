// bdc 0x08881f98 BtlAttackType6EUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x6e (entry 110 of the attack handler table, run by
   `BtlAttackUpdate`): a homing projectile carrying effect 0x278. After 120 frames it stops the
   effect and ends; frame 0 only clears `effect`. Later frames steer (`BtlAttackSteerToTarget`,
   speed 60, height offset 120, decaying turn), test clashes, and from frame 2 sweep-test the
   move; on a hit it spawns effect 0xc5 at the position, stops 0x278, plays sound 0x200099 and
   ends. Otherwise it orients effect 0x278 along the direction and moves the position by the
   velocity (x, y, z; w kept). */
void BtlAttackType6EUpdate(BtlAttack *self)
{
    float mtx[4][4];
    float lenSq;
    float scale;

    if (self->age > 120) {
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x278, self->pos);
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        self->effect = NULL;
    }
    if (self->age == 0) {
        return;
    }
    BtlAttackSteerToTarget(60.0f, 120.0f, self, 1, NULL);
    BtlAttackCheckClash(self);
    if (self->age >= 2 &&
        BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x91, 3, 0, 0x31bf337e) != 0) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0xc5, self->pos);
        GfxEffectStopAttached(g_btlAttackEffectMgr, 0x278, self->pos);
        BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
        BtlAttackEnd(self);
        return;
    }
    /* Look-along matrix: z = normalise(dir), x = normalise(up x z), y = z x x (each normalise
       scales by 1/sqrt(len^2), or 0 for a zero length, clamped to [-1, 1]); w lanes 0,
       w row (0, 0, 0, 1). */
    lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1] + self->dir[2] * self->dir[2];
    scale = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        scale = 0.0f;
    }
    mtx[2][0] = VfSat1(self->dir[0] * scale);
    mtx[2][1] = VfSat1(self->dir[1] * scale);
    mtx[2][2] = VfSat1(self->dir[2] * scale);
    mtx[0][0] = g_vecUp.y * mtx[2][2] - g_vecUp.z * mtx[2][1];
    mtx[0][1] = g_vecUp.z * mtx[2][0] - g_vecUp.x * mtx[2][2];
    mtx[0][2] = g_vecUp.x * mtx[2][1] - g_vecUp.y * mtx[2][0];
    lenSq = mtx[0][0] * mtx[0][0] + mtx[0][1] * mtx[0][1] + mtx[0][2] * mtx[0][2];
    scale = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        scale = 0.0f;
    }
    mtx[0][0] = VfSat1(mtx[0][0] * scale);
    mtx[0][1] = VfSat1(mtx[0][1] * scale);
    mtx[0][2] = VfSat1(mtx[0][2] * scale);
    mtx[1][0] = mtx[2][1] * mtx[0][2] - mtx[2][2] * mtx[0][1];
    mtx[1][1] = mtx[2][2] * mtx[0][0] - mtx[2][0] * mtx[0][2];
    mtx[1][2] = mtx[2][0] * mtx[0][1] - mtx[2][1] * mtx[0][0];
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;
    GfxEffectSetMatrixAttached(g_btlAttackEffectMgr, 0x278, &mtx[0][0], self->pos);
    GfxEffectSetDirAttached(g_btlAttackEffectMgr, 0x278, self->dir, self->pos);
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}

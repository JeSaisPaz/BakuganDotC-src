// bdc 0x088798a4 BtlAttackUpdateTrailedHomingShot
#include "bdc.h"

/* Orientation basis of `dir` with `g_vecUp` written into the 4x4 matrix `m` (fields as columns):
   forward = dir normalised, side = up x forward normalised, up' = forward x side; each normalised
   lane clamped to [-1, 1], a zero-length vector scaled by 0 (the bank's S713); w lanes 0 and the
   last column (0, 0, 0, 1). */
static void BtlAttackTrailedShotBasis(float *m, const float *dir)
{
    float f[3];
    float s[3];
    float lenSq;
    float k;

    lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    f[0] = VfSat1(dir[0] * k);
    f[1] = VfSat1(dir[1] * k);
    f[2] = VfSat1(dir[2] * k);
    s[0] = g_vecUp.y * f[2] - g_vecUp.z * f[1];
    s[1] = g_vecUp.z * f[0] - g_vecUp.x * f[2];
    s[2] = g_vecUp.x * f[1] - g_vecUp.y * f[0];
    lenSq = s[0] * s[0] + s[1] * s[1] + s[2] * s[2];
    k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
    s[0] = VfSat1(s[0] * k);
    s[1] = VfSat1(s[1] * k);
    s[2] = VfSat1(s[2] * k);
    m[0] = s[0];
    m[1] = s[1];
    m[2] = s[2];
    m[3] = 0.0f;
    m[4] = f[1] * s[2] - f[2] * s[1];
    m[5] = f[2] * s[0] - f[0] * s[2];
    m[6] = f[0] * s[1] - f[1] * s[0];
    m[7] = 0.0f;
    m[8] = f[0];
    m[9] = f[1];
    m[10] = f[2];
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

/* m = m * g_gfxSwapYZMatrix (`vmmul.q M000, M100, M200`, column-major as `MathMat4Mul`): column j
   of the result is the sum over k of swap[j][k] * column k of m. */
static void BtlAttackTrailedShotMulSwapYZ(float *m)
{
    const float *b = &g_gfxSwapYZMatrix.x.x;
    float r[16];
    int j;
    int i;

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            r[j * 4 + i] = b[j * 4 + 0] * m[0 * 4 + i] + b[j * 4 + 1] * m[1 * 4 + i] +
                           b[j * 4 + 2] * m[2 * 4 + i] + b[j * 4 + 3] * m[3 * 4 + i];
        }
    }
    for (i = 0; i < 16; i++) {
        m[i] = r[i];
    }
}

/* Shared update of a 30-frame homing shot (attack types 0x18, 0x1a, 0x5f): on the first frame
   spawns the directed muzzle effect 0xb3 and orients its matrix along the direction; afterwards,
   unless `BtlAttackResolveClash` (impact 0xb8) consumed it, homes at speed 100 / height 60
   (`BtlAttackSteerToTarget`), sweeps for hits (`hitId`, kind 3) queuing impact 0xb8, and draws
   five trail effects 0xb2 along its path (from `pos`, 0.2 x `vel` apart) before advancing `pos`
   by `vel`. The muzzle matrix is the orthonormal basis of `dir` with g_vecUp, its rows scaled by
   (1, 1, -1), times g_gfxSwapYZMatrix. */
void BtlAttackUpdateTrailedHomingShot(BtlAttack *self, s32 hitId)
{
    GfxEffect *muzzle;
    float trailPos[4];
    float trailStep[4];
    s32 i;

    if (self->age > 30) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        muzzle = GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xb3, self->pos, self->dir);
        BtlAttackTrailedShotBasis(muzzle->matrix, self->dir);
        /* rows 0..2 scaled by (1, 1, -1), all four lanes */
        for (i = 0; i < 4; i++) {
            muzzle->matrix[0 + i] = muzzle->matrix[0 + i] * 1.0f;
            muzzle->matrix[4 + i] = muzzle->matrix[4 + i] * 1.0f;
            muzzle->matrix[8 + i] = muzzle->matrix[8 + i] * -1.0f;
        }
        BtlAttackTrailedShotMulSwapYZ(muzzle->matrix);
        return;
    }
    if (BtlAttackResolveClash(self, 0xb8) != 0) {
        return;
    }
    BtlAttackSteerToTarget(100.0f, 60.0f, self, 1, NULL);
    if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitId, 3, 0, 0x31bf337e) != 0) {
        BtlAttackSetPendingHit(self, 0xb8, &g_btlAttackHitPoint.x);
        return;
    }
    /* trailPos = pos; trailStep.xyz = vel.xyz * 0.2, through C710 so w = S713 = 0 */
    trailPos[0] = self->pos[0];
    trailPos[1] = self->pos[1];
    trailPos[2] = self->pos[2];
    trailPos[3] = self->pos[3];
    trailStep[0] = self->vel[0] * 0.200000003f;
    trailStep[1] = self->vel[1] * 0.200000003f;
    trailStep[2] = self->vel[2] * 0.200000003f;
    trailStep[3] = 0.0f;
    for (i = 0; i < 5; i++) {
        GfxEffectSpawn(g_btlAttackEffectMgr, 0xb2, trailPos);
        /* trailPos.xyz += trailStep.xyz */
        trailPos[0] = trailPos[0] + trailStep[0];
        trailPos[1] = trailPos[1] + trailStep[1];
        trailPos[2] = trailPos[2] + trailStep[2];
    }
    /* pos.xyz += vel.xyz (w kept) */
    self->pos[0] = self->pos[0] + self->vel[0];
    self->pos[1] = self->pos[1] + self->vel[1];
    self->pos[2] = self->pos[2] + self->vel[2];
}

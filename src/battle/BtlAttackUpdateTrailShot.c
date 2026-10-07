// bdc 0x08879b5c BtlAttackUpdateTrailShot
#include "bdc.h"

/* Orientation basis of `dir` with `g_vecUp` written into the 4x4 matrix `m` (fields as columns):
   forward = dir normalised, side = up x forward normalised, up' = forward x side; each normalised
   lane clamped to [-1, 1], a zero-length vector scaled by 0 (the bank's S713); w lanes 0 and the
   last column (0, 0, 0, 1). */
static void BtlAttackTrailShotBasis(float *m, const float *dir)
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
static void BtlAttackTrailShotMulSwapYZ(float *m)
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

/* Shared update of a 60-frame trail shot (attack types 0x19 and 0x60; `hitKind` is the sweep's hit
   id). Ends the attack once its age passes 60. On the first frame it turns `dir` about the Y axis
   by +-pi/3 (negative when paramF2 != 0), sets the turn rate to 0.21, spawns the directed muzzle
   effect 0xb3 and orients its matrix along `dir` (rows scaled by (1.5, 1.5, -1.5), times
   g_gfxSwapYZMatrix). Later frames: unless BtlAttackResolveClash (impact 0xb8) consumed it, homes
   at speed 35 / height 60 (BtlAttackSteerToTarget), sweeps for hits (kind 3) queuing impact 0xb8
   and returning, else spawns three trail effects 0xb2 from `pos`, 0.33 x `vel` apart, decays the
   turn rate by 0.02 while it is above 0.1 and advances `pos` by `vel`. Every frame that reaches
   the end re-orients the attached effect's matrix along `dir` (basis with g_vecUp, times
   g_gfxSwapYZMatrix). */
void BtlAttackUpdateTrailShot(BtlAttack *self, s32 hitKind)
{
    GfxEffect *muzzle;
    GfxEffect *effect;
    float angle;
    float c;
    float s;
    float dx;
    float dy;
    float dz;
    float trailPos[4];
    float trailStep[4];
    s32 i;

    if (!((float)self->age <= 60.0f)) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        angle = 1.04719758f; /* 0x3f860a92, pi/3 */
        if (self->paramF2 != 0.0f) {
            angle = -angle;
        }
        /* vrot of angle * S703 (2/pi) in quarter turns: dir.xz rotated about Y, y and w kept */
        c = __builtin_cosf(angle);
        s = __builtin_sinf(angle);
        dx = self->dir[0];
        dy = self->dir[1];
        dz = self->dir[2];
        self->dir[0] = dx * c + dy * 0.0f + dz * -s;
        self->dir[2] = dx * s + dy * 0.0f + dz * c;
        self->turnRate = 0.209999993f; /* 0x3e570a3d */
        muzzle = GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xb3, self->pos, self->dir);
        BtlAttackTrailShotBasis(muzzle->matrix, self->dir);
        /* rows 0..2 scaled by (1.5, 1.5, -1.5), all four lanes */
        for (i = 0; i < 4; i++) {
            muzzle->matrix[0 + i] = muzzle->matrix[0 + i] * 1.5f;
            muzzle->matrix[4 + i] = muzzle->matrix[4 + i] * 1.5f;
            muzzle->matrix[8 + i] = muzzle->matrix[8 + i] * -1.5f;
        }
        BtlAttackTrailShotMulSwapYZ(muzzle->matrix);
    } else {
        if (BtlAttackResolveClash(self, 0xb8) != 0) {
            return;
        }
        BtlAttackSteerToTarget(35.0f, 60.0f, self, 1, NULL);
        if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitKind, 3, 0, 0x31bf337e) != 0) {
            BtlAttackSetPendingHit(self, 0xb8, &g_btlAttackHitPoint.x);
            return;
        }
        /* trailPos = pos; trailStep.xyz = vel.xyz * 0.33, through C710 so w = S713 = 0 */
        trailPos[0] = self->pos[0];
        trailPos[1] = self->pos[1];
        trailPos[2] = self->pos[2];
        trailPos[3] = self->pos[3];
        trailStep[0] = self->vel[0] * 0.330000013f; /* 0x3ea8f5c3 */
        trailStep[1] = self->vel[1] * 0.330000013f;
        trailStep[2] = self->vel[2] * 0.330000013f;
        trailStep[3] = 0.0f;
        for (i = 0; i < 3; i++) {
            GfxEffectSpawn(g_btlAttackEffectMgr, 0xb2, trailPos);
            /* trailPos.xyz += trailStep.xyz */
            trailPos[0] = trailPos[0] + trailStep[0];
            trailPos[1] = trailPos[1] + trailStep[1];
            trailPos[2] = trailPos[2] + trailStep[2];
        }
        if (!(self->turnRate <= 0.100000001f)) { /* 0x3dcccccd */
            self->turnRate = self->turnRate - 0.0199999996f; /* 0x3ca3d70a */
        }
        /* pos.xyz += vel.xyz (w kept) */
        self->pos[0] = self->pos[0] + self->vel[0];
        self->pos[1] = self->pos[1] + self->vel[1];
        self->pos[2] = self->pos[2] + self->vel[2];
    }

    effect = (GfxEffect *)self->effect;
    BtlAttackTrailShotBasis(effect->matrix, self->dir);
    effect = (GfxEffect *)self->effect;
    BtlAttackTrailShotMulSwapYZ(effect->matrix);
}

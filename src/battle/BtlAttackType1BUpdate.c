// bdc 0x0887d310 BtlAttackType1BUpdate
#include "bdc.h"

/* Orientation basis of `dir` with `g_vecUp` written into the 4x4 matrix `m` (fields as columns):
   forward = dir normalised, side = up x forward normalised, up' = forward x side; each normalised
   lane clamped to [-1, 1], a zero-length vector scaled by 0 (the bank's S713); w lanes 0 and the
   last column (0, 0, 0, 1). */
static void BtlAttackType1BBasis(float *m, const float *dir)
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

/* m = m * g_gfxSwapYZMatrix (`vmmul.q M000, M100, M200`, column-major): column j of the result is
   the sum over k of swap[j][k] * column k of m. */
static void BtlAttackType1BMulSwapYZ(float *m)
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

/* Per-frame handler of attack type 0x1b (run by BtlAttackUpdate): a 30-frame spread homing shot.
   Ends once its age passes 30. On the first frame it advances g_btlAttackSpreadIndex (reset to 0
   when paramF2 != 0), spawns the directed muzzle effect 0xb3, orients its matrix along `dir`
   (basis with g_vecUp), replaces `dir` by the normalised spread offset
   g_btlAttackSpreadDirs[index % 15] transformed by that basis (w = 0), re-orients the muzzle along
   the new `dir` (rows scaled by (1, 1, -1), times g_gfxSwapYZMatrix) and sets the turn rate to 0.04.
   Later frames: multiplies the turn rate by 0.95, homes at speed 100 / height 60
   (BtlAttackSteerToTarget) and sweeps for hits (id 0x3e, kind 3); on a hit it spawns impact 0xb8
   at the hit point when a collider was hit (g_btlAttackHitCollider), else 0xb9 plus sound
   0x200099, and ends; otherwise advances `pos.xyz` by `vel`. Every frame that does not end
   re-orients the attached effect's matrix along `dir` (times g_gfxSwapYZMatrix). */
void BtlAttackType1BUpdate(BtlAttack *self)
{
    GfxEffect *muzzle;
    s32 index;
    ScePspFVector4 spread;
    float d[3];
    float lenSq;
    float k;
    int i;

    if (self->age > 30) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        index = 0;
        if (self->paramF2 == 0.0f) {
            index = g_btlAttackSpreadIndex + 1;
        }
        g_btlAttackSpreadIndex = index;
        spread = g_btlAttackSpreadDirs[index % 15];
        muzzle = GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xb3, self->pos, self->dir);
        BtlAttackType1BBasis(muzzle->matrix, self->dir);
        /* vtfm3.t with the basis fields as columns: d = spread.x*side + spread.y*up' + spread.z*fwd */
        for (i = 0; i < 3; i++) {
            d[i] = muzzle->matrix[0 + i] * spread.x + muzzle->matrix[4 + i] * spread.y +
                   muzzle->matrix[8 + i] * spread.z;
        }
        /* dir = d normalised (lanes clamped to [-1, 1], zero length scaled by S713 = 0); w is the
           basis' 0 lane, then S713 = 0 */
        lenSq = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        self->dir[0] = VfSat1(d[0] * k);
        self->dir[1] = VfSat1(d[1] * k);
        self->dir[2] = VfSat1(d[2] * k);
        self->dir[3] = 0.0f;
        BtlAttackType1BBasis(muzzle->matrix, self->dir);
        /* fields scaled by (1, 1, -1): only the third changes */
        for (i = 0; i < 4; i++) {
            muzzle->matrix[8 + i] = muzzle->matrix[8 + i] * -1.0f;
        }
        BtlAttackType1BMulSwapYZ(muzzle->matrix);
        self->turnRate = 0.0399999991f; /* 0x3d23d70a */
    } else {
        self->turnRate = self->turnRate * 0.949999988f; /* 0x3f733333 */
        BtlAttackSteerToTarget(100.0f, 60.0f, self, 1, NULL);
        if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, 0x3e, 3, 0, 0x31bf337e) != 0) {
            if (g_btlAttackHitCollider != NULL) {
                GfxEffectSpawn(g_btlAttackEffectMgr, 0xb8, &g_btlAttackHitPoint.x);
            } else {
                GfxEffectSpawn(g_btlAttackEffectMgr, 0xb9, &g_btlAttackHitPoint.x);
                BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
            }
            BtlAttackEnd(self);
            return;
        }
        self->pos[0] = self->pos[0] + self->vel[0];
        self->pos[1] = self->pos[1] + self->vel[1];
        self->pos[2] = self->pos[2] + self->vel[2];
    }

    BtlAttackType1BBasis(((GfxEffect *)self->effect)->matrix, self->dir);
    BtlAttackType1BMulSwapYZ(((GfxEffect *)self->effect)->matrix);
}

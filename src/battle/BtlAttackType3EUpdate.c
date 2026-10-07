// bdc 0x0888317c BtlAttackType3EUpdate
#include "bdc.h"

/* dir.x/z rotated about Y by `angle` radians (`vrot` of angle * S703 in quarter turns, rows
   (c, 0, -s) and (s, 0, c) dotted with dir); y and w kept. */
static void BtlAttackType3ETurnDir(float *dir, float angle)
{
    float c;
    float s;
    float x;
    float y;
    float z;

    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    x = dir[0];
    y = dir[1];
    z = dir[2];
    dir[0] = x * c + y * 0.0f + z * -s;
    dir[2] = x * s + y * 0.0f + z * c;
}

/* Orientation basis of `dir` with `g_vecUp` written into the 4x4 matrix `m` (fields as columns):
   forward = dir normalised, side = up x forward normalised, up' = forward x side; each normalised
   lane clamped to [-1, 1], a zero-length vector scaled by 0 (the bank's S713); w lanes 0 and the
   last column (0, 0, 0, 1). */
static void BtlAttackType3EBasis(float *m, const float *dir)
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
static void BtlAttackType3EMulSwapYZ(float *m)
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

/* Per-frame handler of attack types 0x3e and 0x3f (entry 62 of the handler table `0x08a685f0` run
   by `BtlAttackUpdate`): a wobbling homing shot. Ends once its age passes 120. The homing speed
   lives in `mtx[0][0]`. Frame 0: type 0x3e sets speed 20 and, by side variant `(s32)paramF2`,
   lifts `dir.y` by 0.2 (side 0) or turns `dir` about Y by -/+20 degrees (sides 1/2), then
   `dir.y += 0.4` with turn rate 0; type 0x3f sets speed 40, lowers `pos.y` by 20 (side 0) or
   lifts `dir.y` by 0.2 and turns it by -/+40 degrees (sides 1/2), then `dir.y += 0.3`, moves `pos`
   by 20 * `vel` and sets turn rate 0. Both normalise `dir` (w set to 0) and pick a random wobble
   phase `paramF0` (`CoreRandAngle2Pi`). Later frames: type 0x3e before age 30 raises the turn
   rate by 0.04 from age 13 while below 0.3; otherwise it decays by 0.01 (reset to 0 unless the
   result's bits are a positive integer, i.e. a positive float). The speed grows by 1 up to 30. A
   wobble offset (cos, 0, sin)(paramF0) * ((0.2 - turnRate) * 1500 + 100) is built, the phase
   advances by 0.02 + `CoreRandFloat`(0.06), and the shot homes (`BtlAttackSteerToTarget`):
   type 0x3f with height 80, turn decay and no offset, type 0x3e with height
   (0.3 - turnRate) * 500 + 80 and the wobble offset. Then `BtlAttackResolveClash` (impact 0xc5,
   0x9b for 0x3f) may end it; a sweep hit (kind 0x61 / 0x62, `BtlAttackSweepHit`) sets a pending
   hit at `g_btlAttackHitPoint` (`BtlAttackSetPendingHit`); else `pos.xyz += vel.xyz`. Every
   frame that continues orients the attached effect's matrix along `dir` (basis with `g_vecUp`)
   and multiplies it by `g_gfxSwapYZMatrix`. */
void BtlAttackType3EUpdate(BtlAttack *self)
{
    float offset[4];
    float spread;
    float lenSq;
    float k;
    union { float f; u32 u; } rate;
    s32 side;
    s32 hitKind;
    s32 impact;

    if (!((float)self->age <= 120.0f)) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        if (self->type == 0x3e) {
            side = (s32)self->paramF2;
            self->turnRate = 0.0799999982f;
            if (side > 0) {
                if (side < 2) {
                    BtlAttackType3ETurnDir(self->dir, -0x1.657184p-2f); /* -20 degrees */
                } else if (side < 3) {
                    BtlAttackType3ETurnDir(self->dir, 0x1.657184p-2f); /* +20 degrees */
                }
            } else if (side == 0) {
                self->dir[1] = self->dir[1] + 0.200000003f;
            }
            self->turnRate = 0.0f;
            self->mtx[0][0] = 20.0f;
            self->dir[1] = self->dir[1] + 0.400000006f;
        } else {
            side = (s32)self->paramF2;
            if (side > 0) {
                if (side < 2) {
                    self->dir[1] = self->dir[1] + 0.200000003f;
                    BtlAttackType3ETurnDir(self->dir, -0x1.657184p-1f); /* -40 degrees */
                } else if (side < 3) {
                    self->dir[1] = self->dir[1] + 0.200000003f;
                    BtlAttackType3ETurnDir(self->dir, 0x1.657184p-1f); /* +40 degrees */
                }
            } else if (side == 0) {
                self->pos[1] = self->pos[1] - 20.0f;
            }
            self->mtx[0][0] = 40.0f;
            self->dir[1] = self->dir[1] + 0.300000012f;
            /* pos.xyz += 20 * vel.xyz (staged through a local step vector), pos.w kept */
            self->pos[0] = self->pos[0] + self->vel[0] * 20.0f;
            self->pos[1] = self->pos[1] + self->vel[1] * 20.0f;
            self->pos[2] = self->pos[2] + self->vel[2] * 20.0f;
            self->turnRate = 0.0f;
        }
        /* dir = normalize(dir), lanes clamped to [-1, 1]; the stored w lane is S713 (0) */
        lenSq = self->dir[0] * self->dir[0] + self->dir[1] * self->dir[1] + self->dir[2] * self->dir[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        self->dir[0] = VfSat1(self->dir[0] * k);
        self->dir[1] = VfSat1(self->dir[1] * k);
        self->dir[2] = VfSat1(self->dir[2] * k);
        self->dir[3] = 0.0f;
        self->paramF0 = CoreRandAngle2Pi();
    } else {
        if (self->type == 0x3e && (float)self->age < 30.0f) {
            if (self->age >= 13 && self->turnRate < 0.300000012f) {
                self->turnRate = self->turnRate + 0.0399999991f;
            }
        } else {
            rate.f = self->turnRate - 0.00999999978f;
            if ((s32)rate.u <= 0) {
                rate.f = 0.0f;
            }
            self->turnRate = rate.f;
        }
        if (self->mtx[0][0] < 30.0f) {
            self->mtx[0][0] = self->mtx[0][0] + 1.0f;
        }
        spread = (0.200000003f - self->turnRate) * 1500.0f + 100.0f;
        /* offset = (cos, 0, sin, 0)(paramF0) * spread on x/y/z (vrot of paramF0 * S703) */
        offset[0] = __builtin_cosf(self->paramF0) * spread;
        offset[1] = 0.0f * spread;
        offset[2] = __builtin_sinf(self->paramF0) * spread;
        offset[3] = 0.0f;
        self->paramF0 = CoreRandFloat(0.0599999987f) + 0.0199999996f + self->paramF0;
        if (self->type == 0x3f) {
            BtlAttackSteerToTarget(self->mtx[0][0], 80.0f, self, 1, NULL);
        } else {
            BtlAttackSteerToTarget(self->mtx[0][0], (0.300000012f - self->turnRate) * 500.0f + 80.0f,
                                   self, 0, offset);
        }
        hitKind = 0x61;
        impact = 0xc5;
        if (self->type == 0x3f) {
            hitKind = 0x62;
            impact = 0x9b;
        }
        if (BtlAttackResolveClash(self, impact) != 0) {
            return;
        }
        if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitKind, 3, 0, 0x31bf337e) !=
            0) {
            BtlAttackSetPendingHit(self, impact, &g_btlAttackHitPoint.x);
            return;
        }
        self->pos[0] = self->pos[0] + self->vel[0];
        self->pos[1] = self->pos[1] + self->vel[1];
        self->pos[2] = self->pos[2] + self->vel[2];
    }

    BtlAttackType3EBasis(((GfxEffect *)self->effect)->matrix, self->dir);
    BtlAttackType3EMulSwapYZ(((GfxEffect *)self->effect)->matrix);
}

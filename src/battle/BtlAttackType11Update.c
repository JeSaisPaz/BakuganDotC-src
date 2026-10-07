// bdc 0x0887c7a4 BtlAttackType11Update
#include "bdc.h"

/* Per-frame handler of attack type 0x11 (handler table `0x08a685f0`, run by `BtlAttackUpdate`):
   a bone-attached beam like `BtlAttackType0EUpdate`. Phase 0 clears the length `paramF0` and
   enters phase 1, which grows the length by 20% of its distance to 240 per frame until it passes
   200 (clamped to 200, phase 2). Phase 2, while `age` < 46, holds the attack at age 28 as long as
   the owner is in states 3..6; afterwards the attached effect fades (alpha -0.1 per frame) and the
   attack ends (`BtlAttackEnd`) once the alpha is <= 0. Negative phases and phases above 2 skip
   both steps. Every frame that does not end it sets the effect height to the length and its
   `vec1d0.x` to 2, follows the owner's bone (`rootMatrix` x bone `localMatrix` into `mtx`), sets
   `pos` = `mtx` x `g_btlAttackType11Offset` (w = 1) and `auxVec` = the normalised rotation of
   `g_btlAttackType11Dir`; when the target unit (`BtlFindBakuganById`) exists, `auxVec.y` is
   replaced by the y of the normalised owner-to-target vector. `segment` = `auxVec` x (length x 12);
   `vel` is set to `auxVec` on frame 0 and otherwise moves 20% towards it, and is copied to the
   effect's `dir`. Before age 52 it sweeps the beam (`BtlAttackSweepHit`, hit kind 0x34, mask
   `0x31bf337e`): a unit hit spawns directed effect 0xa1 at the hit point with sound `0x200099` on
   every other unit hit (`param1`), a world hit does so 3 units out along the contact normal on odd
   `param0` counts; `param0` counts all hits. Then `BtlAttackCheckClash`. The normalisations
   (clamped to [-1, 1]) scale a zero vector by 0, and `auxVec.w`/`segment.w` are set to 0 (the VFPU
   bank's S713 lane under the scale). */
void BtlAttackType11Update(BtlAttack *self)
{
    GfxEffect *effect;
    BtlBakugan *target;
    float spawnPos[4];
    float v[3];
    const float *root;
    const float *local;
    float lenSq;
    float scale;
    int i;
    int j;
    float length;

    if (self->phase == 0) {
        self->paramF0 = 0.0f;
        self->phase++;
    }
    if (self->phase == 1) {
        length = self->paramF0 + (240.0f - self->paramF0) * 0.200000003f;
        self->paramF0 = length;
        if (!(length <= 200.0f)) {
            self->paramF0 = 200.0f;
            self->phase++;
        }
    } else if (self->phase == 2) {
        if (self->age < 0x2e) {
            if (self->owner->state >= 3 && self->owner->state < 7) {
                self->age = 0x1c;
            }
        } else {
            effect = self->effect;
            effect->color[3] -= 0.100000001f;
            if (effect->color[3] <= 0.0f) {
                BtlAttackEnd(self);
                return;
            }
        }
    }
    effect = self->effect;
    effect->size[1] = self->paramF0;
    effect->vec1d0[0] = 2.0f;

    if (g_btlAttackType11OffsetReady == 0) {
        g_btlAttackType11OffsetReady = 1;
        g_btlAttackType11Offset.x = 18.0f;
        g_btlAttackType11Offset.y = -9.0f;
        g_btlAttackType11Offset.z = 0.0f;
        g_btlAttackType11Offset.w = 0.0f;
    }
    if (g_btlAttackType11DirReady == 0) {
        g_btlAttackType11DirReady = 1;
        g_btlAttackType11Dir.x = 1.0f;
        g_btlAttackType11Dir.y = -0.200000003f;
        g_btlAttackType11Dir.z = 0.0f;
        g_btlAttackType11Dir.w = 0.0f;
    }

    /* mtx = owner root matrix x bone local matrix (column j = sum over k of local[j][k] x root
       column k); pos = mtx x (offset.xyz, 1); auxVec = clamp(normalise(mtx3x3 x dir)) with w = 0
       (bank S713; a zero vector scales by S713 = 0 too) */
    root = self->owner->base.data->rootMatrix;
    local = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            self->mtx[j][i] = local[j * 4 + 0] * root[0 * 4 + i] + local[j * 4 + 1] * root[1 * 4 + i] +
                              local[j * 4 + 2] * root[2 * 4 + i] + local[j * 4 + 3] * root[3 * 4 + i];
        }
    }
    for (i = 0; i < 4; i++) {
        self->pos[i] = self->mtx[0][i] * g_btlAttackType11Offset.x + self->mtx[1][i] * g_btlAttackType11Offset.y +
                       self->mtx[2][i] * g_btlAttackType11Offset.z + self->mtx[3][i] * 1.0f;
    }
    for (i = 0; i < 3; i++) {
        v[i] = self->mtx[0][i] * g_btlAttackType11Dir.x + self->mtx[1][i] * g_btlAttackType11Dir.y +
               self->mtx[2][i] * g_btlAttackType11Dir.z;
    }
    lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    scale = VfRsq(lenSq);
    if (lenSq == 0.0f) {
        scale = 0.0f;
    }
    self->auxVec[0] = VfSat1(v[0] * scale);
    self->auxVec[1] = VfSat1(v[1] * scale);
    self->auxVec[2] = VfSat1(v[2] * scale);
    self->auxVec[3] = 0.0f;

    target = (BtlBakugan *)BtlFindBakuganById(self->targetId);
    if (target != NULL) {
        /* auxVec.y = y of clamp(normalise(target pos - owner pos)) */
        v[0] = target->base.pos[0] - self->owner->base.pos[0];
        v[1] = target->base.pos[1] - self->owner->base.pos[1];
        v[2] = target->base.pos[2] - self->owner->base.pos[2];
        lenSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
        scale = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            scale = 0.0f;
        }
        self->auxVec[1] = VfSat1(v[1] * scale);
    }

    /* segment = auxVec x (length x 12), w = 0 (bank S713) */
    scale = self->paramF0 * 12.0f;
    self->segment[0] = self->auxVec[0] * scale;
    self->segment[1] = self->auxVec[1] * scale;
    self->segment[2] = self->auxVec[2] * scale;
    self->segment[3] = 0.0f;
    if (self->age == 0) {
        for (i = 0; i < 4; i++) {
            self->vel[i] = self->auxVec[i];
        }
    } else {
        /* vel += (auxVec - vel) x 0.2 (all four lanes) */
        for (i = 0; i < 4; i++) {
            self->vel[i] = self->vel[i] + (self->auxVec[i] - self->vel[i]) * 0.200000003f;
        }
    }
    effect = self->effect;
    for (i = 0; i < 4; i++) {
        effect->dir[i] = self->vel[i];
    }

    if (self->age < 0x34 &&
        BtlAttackSweepHit(self->radius, self, self->pos, self->segment, 0x34, 0, 1, 0x31bf337e)) {
        if (g_btlAttackHitCollider != NULL) {
            if (self->param1 % 2 == 0) {
                GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xa1, &g_btlAttackHitPoint.x, self->vel);
                BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
            }
            self->param1++;
        } else if ((self->param0 & 1) != 0) {
            /* spawnPos = hit point + contact normal x 3 (xyz, w from the hit point) */
            spawnPos[0] = g_btlAttackHitPoint.x + g_collisionHitResult.normal.x * 3.0f;
            spawnPos[1] = g_btlAttackHitPoint.y + g_collisionHitResult.normal.y * 3.0f;
            spawnPos[2] = g_btlAttackHitPoint.z + g_collisionHitResult.normal.z * 3.0f;
            spawnPos[3] = g_btlAttackHitPoint.w;
            GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xa1, spawnPos, &g_collisionHitResult.normal.x);
            BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
        }
        self->param0++;
    }
    BtlAttackCheckClash(self);
}

// bdc 0x0887c438 BtlAttackType0EUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0xe (handler table `0x08a685f0`, run by `BtlAttackUpdate`): a
   bone-attached beam like `BtlAttackType11Update`. Phase 0 clears the length `paramF0`, saves
   `pos` in `auxVec` and enters phase 1, which grows the length by 20% of its distance to 240 per
   frame until it passes 200 (clamped to 200, phase 2). Phase 2, while `age` < 46, holds the attack
   at age 28 as long as the owner is in states 3..6; afterwards the attached effect fades (alpha
   -0.1 per frame) and the attack ends (`BtlAttackEnd`) once the alpha is <= 0. Every frame that
   does not end it sets the effect height to the length, follows the owner's bone
   (`rootMatrix` x bone `localMatrix` into `mtx`, `pos` = its translation), sets `segment` = `vel`
   x (length x 10) and `auxVec` = beam end. Before age 45 it sweeps the beam (`BtlAttackSweepHit`,
   hit kind 0x31, mask `0x31bf337e`): a unit hit spawns directed effect 0xa1 at the hit point with
   sound `0x200099` on every other unit hit (`param1`), a world hit does so 3 units out along the
   contact normal on odd `param0` counts; `param0` counts all hits. Then `BtlAttackCheckClash`.
   `segment.w` is set to 0 (the VFPU bank's S713 lane under the scale). */
void BtlAttackType0EUpdate(BtlAttack *self)
{
    GfxEffect *effect;
    float spawnPos[4];
    const float *root;
    const float *local;
    float length;
    float scale;
    int i;
    int j;

    if (self->phase == 0) {
        self->paramF0 = 0.0f;
        self->auxVec[0] = self->pos[0];
        self->auxVec[1] = self->pos[1];
        self->auxVec[2] = self->pos[2];
        self->auxVec[3] = self->pos[3];
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

    /* mtx = owner root matrix x bone local matrix (column j = sum over k of local[j][k] x root
       column k); pos = mtx row 3; segment = vel x (length x 10) with w = 0 (bank S713); auxVec =
       pos + segment (xyz, w from pos) */
    root = self->owner->base.data->rootMatrix;
    local = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            self->mtx[j][i] = local[j * 4 + 0] * root[0 * 4 + i] + local[j * 4 + 1] * root[1 * 4 + i] +
                              local[j * 4 + 2] * root[2 * 4 + i] + local[j * 4 + 3] * root[3 * 4 + i];
        }
    }
    self->pos[0] = self->mtx[3][0];
    self->pos[1] = self->mtx[3][1];
    self->pos[2] = self->mtx[3][2];
    self->pos[3] = self->mtx[3][3];
    scale = self->paramF0 * 10.0f;
    self->segment[0] = self->vel[0] * scale;
    self->segment[1] = self->vel[1] * scale;
    self->segment[2] = self->vel[2] * scale;
    self->segment[3] = 0.0f;
    self->auxVec[0] = self->pos[0] + self->segment[0];
    self->auxVec[1] = self->pos[1] + self->segment[1];
    self->auxVec[2] = self->pos[2] + self->segment[2];
    self->auxVec[3] = self->pos[3];

    if (self->age < 0x2d &&
        BtlAttackSweepHit(self->radius, self, self->pos, self->segment, 0x31, 0, 1, 0x31bf337e)) {
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

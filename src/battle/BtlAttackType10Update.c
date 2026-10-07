// bdc 0x0887c090 BtlAttackType10Update
#include "bdc.h"

/* Per-frame handler of attack type 0x10 (handler table `0x08a685f0`, run by `BtlAttackUpdate`): an
   owner-held blade. Phase 0 clears the length `paramF0` and enters phase 1, which grows it by 20%
   of its distance to 240 per frame until it passes 200 (clamped to 200, phase 2). Phase 2, while
   `age` < 46, holds the attack at age 28 as long as the owner is in states 3..6; afterwards the
   attached effect fades (alpha -0.1 per frame) and the attack ends (`BtlAttackEnd`) once the
   alpha is <= 0. Every frame that does not end it sets the effect height to the length and its
   `vec1d0.x` to 2, follows the owner's bone (`rootMatrix` x bone `localMatrix` into `mtx`), puts
   `pos` at the bone-local point `g_btlAttackType10TipOffset` (90, 0, 0) and sets `segment` =
   `vel` x (length x 12). Before age 52 it sweeps the blade (`BtlAttackSweepHit`, hit kind 0x33,
   mask `0x31bf337e`) and spawns directed impact effect 0x1b with sound `0x200099` on
   every third unit hit, or every fourth world hit 3 units out along the contact normal (`param1`
   counts both). Then `BtlAttackCheckClash`. `segment.w` is 0 (the `vscl.t` result's w lane is
   the VFPU bank constant S713 = 0). */
void BtlAttackType10Update(BtlAttack *self)
{
    GfxEffect *effect;
    float spawnPos[4] __attribute__((aligned(16)));
    const float *root;
    const float *local;
    float length;
    int i;
    int j;

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
    effect = self->effect;
    effect->vec1d0[0] = 2.0f;
    if (g_btlAttackType10TipInited == 0) {
        g_btlAttackType10TipInited = 1;
        g_btlAttackType10TipOffset[1] = 0.0f;
        g_btlAttackType10TipOffset[0] = 90.0f;
        g_btlAttackType10TipOffset[2] = 0.0f;
        g_btlAttackType10TipOffset[3] = 0.0f;
    }

    /* mtx = owner root matrix x bone matrix (column-major) */
    root = self->owner->base.data->rootMatrix;
    local = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            self->mtx[j][i] = local[j * 4 + 0] * root[i] + local[j * 4 + 1] * root[4 + i] +
                              local[j * 4 + 2] * root[8 + i] + local[j * 4 + 3] * root[12 + i];
        }
    }
    /* pos = mtx * (tip offset xyz, 1) */
    for (i = 0; i < 4; i++) {
        self->pos[i] = g_btlAttackType10TipOffset[0] * self->mtx[0][i] +
                       g_btlAttackType10TipOffset[1] * self->mtx[1][i] +
                       g_btlAttackType10TipOffset[2] * self->mtx[2][i] + self->mtx[3][i];
    }
    /* segment = (vel.xyz * length * 12, 0) */
    self->segment[0] = self->vel[0];
    self->segment[1] = self->vel[1];
    self->segment[2] = self->vel[2];
    length = self->paramF0 * 12.0f;
    self->segment[0] = self->segment[0] * length;
    self->segment[1] = self->segment[1] * length;
    self->segment[2] = self->segment[2] * length;
    self->segment[3] = 0.0f;

    if (self->age < 0x34 &&
        BtlAttackSweepHit(self->radius, self, self->pos, self->segment, 0x33, 0, 1, 0x31bf337e)) {
        if (g_btlAttackHitCollider != NULL) {
            if (self->param1 % 3 == 0) {
                GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0x1b, &g_btlAttackHitPoint.x, self->vel);
                BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
            }
        } else if (self->param1 % 4 == 0) {
            /* spawnPos = hit point + contact normal * 3 (xyz, w from the hit point) */
            spawnPos[0] = g_btlAttackHitPoint.x + g_collisionHitResult.normal.x * 3.0f;
            spawnPos[1] = g_btlAttackHitPoint.y + g_collisionHitResult.normal.y * 3.0f;
            spawnPos[2] = g_btlAttackHitPoint.z + g_collisionHitResult.normal.z * 3.0f;
            spawnPos[3] = g_btlAttackHitPoint.w;
            GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0x1b, spawnPos, &g_collisionHitResult.normal.x);
            BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
        }
        self->param1++;
    }
    BtlAttackCheckClash(self);
}

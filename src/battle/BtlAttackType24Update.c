// bdc 0x0887e3cc BtlAttackType24Update
#include "bdc.h"

/* Per-frame handler of attack type 0x24 (entry 36 of the handler table run by
   `BtlAttackUpdate`): a beam that follows the owner's bone. Phases: below 0x1e (and from 0x20
   on) resets the length `paramF0` and `effect` and advances; 0x1e grows the length toward 240
   (rate 0.2) and advances once it passes 200 (clamped to 200); 0x1f ends the attack
   (`BtlAttackEnd`) after age 0x3c and holds the age at 0x1c while the owner's state is 3..6.
   Every frame it sets the height (`size[1]`) of its effects 0x21e/0x21f
   (`GfxEffectFindAttached` at `pos`) to the length and their `vec1d0.x` to 3 / 6.5, places `pos`
   at the bone (`MathMat4Mul` of the owner's root and bone matrices into `mtx`, row 3) and sets
   `segment = (vel.xyz * length * 12, 0)` (`segment.w` is the VFPU bank constant S713 = 0). For ages
   11..0x33 it sweeps the beam (`BtlAttackSweepHit`, hit kind 0x47, mask 0x31bf337e): a hit on a
   collider spawns effect 0xc5 at the hit point along `vel` on every second hit, a hit without
   collider spawns it 3 units off the surface along the normal on every third hit, both with
   sound 0x200099 (`BtlAttackPlaySound`). Finally checks clashes (`BtlAttackCheckClash`). */

void BtlAttackType24Update(BtlAttack *self)
{
    GfxEffect *effect;
    float *world;
    float length;
    bool hold;
    float spawnPos[4] __attribute__((aligned(16)));

    if (self->phase < 0x1f && self->phase >= 0x1e) {
        length = self->paramF0 + (240.0f - self->paramF0) * 0.200000003f;
        self->paramF0 = length;
        if (!(length <= 200.0f)) {
            self->paramF0 = 200.0f;
            self->phase = self->phase + 1;
        }
    } else if (self->phase == 0x1f) {
        if (!(self->age < 0x3d)) {
            BtlAttackEnd(self);
            return;
        }
        hold = false;
        if (self->owner->state >= 3 && self->owner->state < 7) {
            hold = true;
        }
        if (hold) {
            self->age = 0x1c;
        }
    } else {
        self->paramF0 = 0.0f;
        self->effect = NULL;
        self->phase = self->phase + 1;
    }
    effect = GfxEffectFindAttached(g_btlAttackEffectMgr, 0x21e, self->pos);
    if (effect != NULL) {
        effect->size[1] = self->paramF0;
        effect->vec1d0[0] = 3.0f;
    }
    effect = GfxEffectFindAttached(g_btlAttackEffectMgr, 0x21f, self->pos);
    if (effect != NULL) {
        effect->size[1] = self->paramF0;
        effect->vec1d0[0] = 6.5f;
    }
    world = (float *)MathMat4Mul((ScePspFMatrix4 *)self->mtx, (const ScePspFMatrix4 *)self->owner->base.data->rootMatrix,
                                 (const ScePspFMatrix4 *)self->bone->localMatrix);
    /* pos = world row 3; segment = (vel.xyz * length * 12, 0) */
    self->pos[0] = world[12];
    self->pos[1] = world[13];
    self->pos[2] = world[14];
    self->pos[3] = world[15];
    self->segment[0] = self->vel[0];
    self->segment[1] = self->vel[1];
    self->segment[2] = self->vel[2];
    length = self->paramF0 * 12.0f;
    self->segment[0] = self->segment[0] * length;
    self->segment[1] = self->segment[1] * length;
    self->segment[2] = self->segment[2] * length;
    self->segment[3] = 0.0f;
    if (self->age < 0x34 && self->age >= 0xb &&
        BtlAttackSweepHit(self->radius, self, self->pos, self->segment, 0x47, 0, 1, 0x31bf337e) != 0) {
        if (g_btlAttackHitCollider != NULL) {
            if (self->param1 % 2 == 0) {
                GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xc5, &g_btlAttackHitPoint.x,
                                       self->vel);
                BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
            }
            self->param1 = self->param1 + 1;
        } else {
            if (self->param1 % 3 == 0) {
                /* spawnPos = hit point + normal * 3 (xyz; w of the hit point) */
                spawnPos[0] = g_btlAttackHitPoint.x + g_collisionHitResult.normal.x * 3.0f;
                spawnPos[1] = g_btlAttackHitPoint.y + g_collisionHitResult.normal.y * 3.0f;
                spawnPos[2] = g_btlAttackHitPoint.z + g_collisionHitResult.normal.z * 3.0f;
                spawnPos[3] = g_btlAttackHitPoint.w;
                GfxEffectSpawnDirected(g_btlAttackEffectMgr, 0xc5, spawnPos,
                                       &g_collisionHitResult.normal.x);
                BtlAttackPlaySound(self, 0x200099, NULL, 0, 0);
            }
            self->param1 = self->param1 + 1;
        }
    }
    BtlAttackCheckClash(self);
}

// bdc 0x08883c34 BtlAttackType80Update
#include "bdc.h"

/* Per-frame handler of attack type 0x80 (entry 128 of the handler table `0x08a685f0` run by
   `BtlAttackUpdate`): a summoning shot that bobs along `vel` and hatches an egg crystal where it
   lands. A cancelled attack spawns effect 0x55 on `g_btlUnitEffectMgr` and ends. On frame 0 it
   runs its effect's virtual slot 2 twice, scales `vel.xyz` by 22 (w = 0, the VFPU bank's S713)
   and sets the bob step `paramF0` = pi/40 and phase `paramF1` = pi/2. Later frames advance the
   phase (clamped to 2 pi), add `16 * sin(phase)` to the step's y, copy the step to the effect's
   `dir` and sweep from `pos` for hits (hit id 0xa3, kind 3, mask `0x3fbf2700`) with the owner's
   body-collider layer saved (restored only when it does not explode). After 150 frames, on a
   hit, or below y = -100 it explodes: plays sound `0x200258` once per battle frame
   (`g_btlAttack80SoundFrame`; through the sound manager when script bit 5 is set, else as a
   positional emitter at `pos`), summons an egg crystal at `pos` (w = `auxVec[3]`) with
   `param0`/`param1`, the truncated `auxVec.xyz` and target `summonVec`
   (`ActorStageObjEggCrystalSummon`) and ends (`BtlAttackEnd`). Otherwise, after frame 0,
   `pos.xyz += step.xyz`. */
void BtlAttackType80Update(BtlAttack *self)
{
    float step[4];
    float at[4];
    float target[4];
    float wave;
    GfxEffect *effect;
    CollisionCollider *body;
    const VtblEntry *entry;
    u32 savedLayer;
    bool muted;

    if (self->age > 150) {
        goto explode;
    }
    if (self->cancelled != 0) {
        GfxEffectSpawn(g_btlUnitEffectMgr, 0x55, self->pos);
        BtlAttackEnd(self);
        return;
    }
    if (self->age == 0) {
        effect = (GfxEffect *)self->effect;
        entry = &((const VtblEntry *)effect->base.vtable)[2];
        ((void (*)(void *))entry->fn)((u8 *)effect + entry->delta);
        effect = (GfxEffect *)self->effect;
        entry = &((const VtblEntry *)effect->base.vtable)[2];
        ((void (*)(void *))entry->fn)((u8 *)effect + entry->delta);
        self->vel[0] = self->vel[0] * 22.0f;
        self->vel[1] = self->vel[1] * 22.0f;
        self->vel[2] = self->vel[2] * 22.0f;
        self->vel[3] = 0.0f; /* S713 of the VFPU bank, stored with the scaled column */
        step[0] = self->vel[0];
        step[1] = self->vel[1];
        step[2] = self->vel[2];
        step[3] = self->vel[3];
        self->paramF0 = 0x1.41b2f8p-4f; /* pi/40 */
        self->paramF1 = 0x1.921fb6p+0f; /* pi/2 */
    } else {
        step[0] = self->vel[0];
        step[1] = self->vel[1];
        step[2] = self->vel[2];
        step[3] = self->vel[3];
        self->paramF1 = self->paramF1 + self->paramF0;
        if (!(self->paramF1 <= 0x1.921fb6p+2f)) {
            self->paramF1 = 0x1.921fb6p+2f; /* 2 pi */
        }
        wave = __builtin_sinf(self->paramF1);
        step[1] = step[1] + wave * 16.0f;
        effect = (GfxEffect *)self->effect;
        effect->dir[0] = step[0];
        effect->dir[1] = step[1];
        effect->dir[2] = step[2];
        effect->dir[3] = step[3];
        g_btlAttackHitCollider = NULL;
        body = self->owner->collider0;
        savedLayer = body->layer;
        if (BtlAttackSweepHit(self->radius, self, self->pos, step, 0xa3, 3, 0, 0x3fbf2700) != 0 ||
            self->pos[1] < -100.0f) {
            goto explode;
        }
        body = self->owner->collider0;
        body->layer = savedLayer;
    }
    if (self->age > 0) {
        self->pos[0] = self->pos[0] + step[0];
        self->pos[1] = self->pos[1] + step[1];
        self->pos[2] = self->pos[2] + step[2];
    }
    return;

explode:
    step[0] = self->pos[0];
    step[1] = self->pos[1];
    step[2] = self->pos[2];
    step[3] = self->auxVec[3];
    muted = CoreBitsetTest(5, g_scriptGlobalBits);
    if (g_btlAttack80SoundFrame != g_btlFrameCount) {
        if (muted) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x200258, 0, 0);
            }
        } else if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), 0x200258, self->pos, 0, 1);
        }
    }
    g_btlAttack80SoundFrame = g_btlFrameCount;
    at[0] = step[0];
    at[1] = step[1];
    at[2] = step[2];
    at[3] = step[3];
    target[0] = self->summonVec[0];
    target[1] = self->summonVec[1];
    target[2] = self->summonVec[2];
    target[3] = self->summonVec[3];
    ActorStageObjEggCrystalSummon(at, self->param0, self->param1, (s32)self->auxVec[0],
                                  (s32)self->auxVec[1], (s32)self->auxVec[2], target);
    BtlAttackEnd(self);
}

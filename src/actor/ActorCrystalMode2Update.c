// bdc 0x0885aa58 ActorCrystalMode2Update
#include "bdc.h"

/* Crystal mode-2 handler (entry 2 of the mode table `0x08a670ec` used by
   `ActorCrystalUpdateLogic`). Does nothing while a cut-in runs (`BtlIsCutInRunning`), when
   there is no player Bakugan (`BtlGetPlayerBakugan`) or when it is dead. Otherwise steps the
   sub-step `step`:
   0: lights the model, spawns effect 0x5b at the crystal (its `vec1e0[0]` = 0.4 * scale x),
      plays sound 0x20025e, clears the ambient alpha and waits 10 frames (1);
   2: fades in: `fade` = sin(min(max(timer * 2deg, 0), pi/2)) is written to the ambient alpha, all
      materials (`GfxModelScaleAmbientColor`) and half of it to `"mat_spel"`; when it reaches 1
      the model is unlit and the handler waits 5 frames (3, 4);
   5: without `mode2Proceed` jumps to step 999 (the default: finish); otherwise
   6: focuses the battle camera on the crystal when script flag 5 is set (`BtlCameraFocusUnit`);
   10: aims at the player: `fireParam` = atan2 of the xz offset, `firePoint` = crystal pos + 250
      along that yaw, raised by 250, sound 0x20025a, a directed effect 0x30 attached to
      `firePoint`; `firePoint2` = flattened unit direction toward the player, blended 30% toward
      the active camera's `dir` and scaled by 2.5;
   11: waits 60 frames; 12: allocates and launches the attack (`BtlAttackCtor(obj, crystal,
      0x7f)`, `BtlAttackLaunchStage` from `firePoint` along `firePoint2`); 13/14: waits 30 frames;
   7-9 and any other step: clears `scriptedCast`/`onstageFlag`, sets `mode2Done` and
   `mode2Proceed` and returns to mode 0 (`ActorCrystalSetMode`).
   VFPU bank constants: S703 = 2/pi (angle -> quarter turns for vsin/vrot) and S713 = 0 (scale
   for a zero-length direction, and the w lane of `firePoint2`). */

void ActorCrystalMode2Update(ActorCrystal *self)
{
    float dir[4];
    float lenSq;
    float k;
    BtlBakugan *player;
    GfxEffect *effect;
    BtlAttack *mem;
    BtlAttack *attack;
    float angle;
    float fade;
    bool fromLow;
    int t;

    player = (BtlBakugan *)BtlGetPlayerBakugan();
    if (BtlIsCutInRunning()) {
        return;
    }
    if (player == NULL) {
        return;
    }
    if (player->combat.dead != 0) {
        return;
    }
    switch (self->step) {
    case 0:
        self->base.base.lighting = 1;
        effect = (GfxEffect *)GfxEffectSpawn(g_btlUnitEffectMgr, 0x5b, self->base.base.pos);
        effect->vec1e0[0] = self->base.base.scale[0] * 0.4f;
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x20025e, 0, 0);
        }
        self->base.base.ambient[3] = 0.0f;
        self->timer = 10;
        self->step = self->step + 1;
        /* fall through */
    case 1:
        t = self->timer;
        self->timer = t - 1;
        if (t > 0) {
            return;
        }
        self->timer = 0;
        self->step = self->step + 1;
        /* fall through */
    case 2:
        t = self->timer;
        angle = (float)t * 0.034906585f;
        self->timer = t + 1;
        if (angle < 0.0f) {
            angle = 0.0f;
        } else if (!(angle <= 1.5707964f)) {
            angle = 1.5707964f;
        }
        /* vsin of angle * 2/pi (bank S703) */
        fade = __builtin_sinf(angle);
        self->base.base.ambient[3] = fade;
        GfxModelScaleAmbientColor(fade, &self->base.base, NULL);
        GfxModelScaleAmbientColorByName(fade * 0.5f, &self->base.base, "mat_spel");
        self->fade = fade;
        if (fade < 1.0f) {
            return;
        }
        self->base.base.lighting = 0;
        self->step = self->step + 1;
        /* fall through */
    case 3:
        self->timer = 5;
        self->step = self->step + 1;
        /* fall through */
    case 4:
        t = self->timer;
        self->timer = t - 1;
        if (t > 0) {
            return;
        }
        self->step = self->step + 1;
        /* fall through */
    case 5:
        if (self->mode2Proceed == 0) {
            self->step = 999;
            return;
        }
        self->step = self->step + 1;
        /* fall through */
    case 6:
        if (CoreBitsetTest(5, g_scriptGlobalBits)) {
            BtlCameraFocusUnit(2000.0f, 500.0f, 40.0f, (BtlMain *)BtlGetCameraTask(), self, 0, 0,
                               NULL);
        }
        self->step = 10;
        /* fall through */
    case 10:
        angle = atan2f(player->base.pos[2] - self->base.base.pos[2],
                       player->base.pos[0] - self->base.base.pos[0]);
        self->fireParam = angle;
        /* vrot [C,0,S,0] of angle * 2/pi (bank S703), scaled by 250 on x, y, z */
        dir[0] = __builtin_cosf(angle) * 250.0f;
        dir[1] = 0.0f;
        dir[2] = __builtin_sinf(angle) * 250.0f;
        dir[3] = 0.0f;
        self->firePoint[0] = dir[0] + self->base.base.pos[0];
        self->firePoint[1] = dir[1] + self->base.base.pos[1];
        self->firePoint[2] = dir[2] + self->base.base.pos[2];
        self->firePoint[3] = dir[3];
        self->firePoint[1] = self->firePoint[1] + 250.0f;
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x20025a, 0, 0);
        }
        effect = GfxEffectSpawnDirected(g_btlUnitEffectMgr, 0x30, self->firePoint, dir);
        effect->attachPos = self->firePoint;
        self->firePoint2[0] = player->base.pos[0] - self->firePoint[0];
        self->firePoint2[1] = player->base.pos[1] - self->firePoint[1];
        self->firePoint2[2] = player->base.pos[2] - self->firePoint[2];
        self->firePoint2[3] = player->base.pos[3];
        self->firePoint2[1] = 0.0f;
        /* normalise (factor 0 for a zero vector, bank S713), clamp to [-1, 1]; w is S713 = 0 */
        lenSq = self->firePoint2[0] * self->firePoint2[0] +
                self->firePoint2[1] * self->firePoint2[1] +
                self->firePoint2[2] * self->firePoint2[2];
        k = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            k = 0.0f;
        }
        self->firePoint2[0] = VfSat1(self->firePoint2[0] * k);
        self->firePoint2[1] = VfSat1(self->firePoint2[1] * k);
        self->firePoint2[2] = VfSat1(self->firePoint2[2] * k);
        self->firePoint2[3] = 0.0f;
        /* blend 30% toward the active camera's dir (all four lanes) */
        self->firePoint2[0] = self->firePoint2[0] +
                              (g_gfxActiveCamera->dir[0] - self->firePoint2[0]) * 0.3f;
        self->firePoint2[1] = self->firePoint2[1] +
                              (g_gfxActiveCamera->dir[1] - self->firePoint2[1]) * 0.3f;
        self->firePoint2[2] = self->firePoint2[2] +
                              (g_gfxActiveCamera->dir[2] - self->firePoint2[2]) * 0.3f;
        self->firePoint2[3] = self->firePoint2[3] +
                              (g_gfxActiveCamera->dir[3] - self->firePoint2[3]) * 0.3f;
        /* scale x, y, z by 2.5; w is the masked lane S713 = 0 */
        self->firePoint2[0] = self->firePoint2[0] * 2.5f;
        self->firePoint2[1] = self->firePoint2[1] * 2.5f;
        self->firePoint2[2] = self->firePoint2[2] * 2.5f;
        self->firePoint2[3] = 0.0f;
        self->timer = 0;
        self->step = self->step + 1;
        return;
    case 11:
        t = self->timer;
        self->timer = t + 1;
        if (t < 0x3b) {
            return;
        }
        self->step = self->step + 1;
        /* fall through */
    case 12:
        attack = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = (BtlAttack *)MemAlloc(0x160, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (mem != NULL) {
            BtlAttackCtor(mem, self, 0x7f);
            attack = mem;
        }
        BtlAttackLaunchStage(attack, self->firePoint, self->firePoint2, NULL);
        self->step = self->step + 1;
        return;
    case 13:
        self->timer = 0x1e;
        self->step = self->step + 1;
        /* fall through */
    case 14:
        t = self->timer;
        self->timer = t - 1;
        if (t > 0) {
            return;
        }
        self->step = self->step + 1;
        /* fall through */
    case 7:
    case 8:
    case 9:
    default:
        self->scriptedCast = 0;
        self->onstageFlag = 0;
        self->mode2Done = 1;
        self->mode2Proceed = 1;
        ActorCrystalSetMode(self, 0, false);
        return;
    }
}

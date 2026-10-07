// bdc 0x0889ee90 BtlStageUpdateAmbientEffects
#include "bdc.h"

/* Per-frame update of the arena colour flash started by a gate card (`BtlStageStartMapFlash`).
   First hides the two sky billboards `g_btlSkySprites` (clears visible bit 0 of `flags`) while
   the cut-in task 0x1e1 exists and shows them otherwise. Then steps `g_btlMapFlashState`. State 1
   (fading in): `g_btlMapFlashLevel` += 1/15; on reaching 1 it is clamped, the state becomes 2 and
   the per-kind looping effect `g_btlMapFlashLoopEffect` is spawned at the origin
   (`GfxEffectSpawnOwned`, owner the first arena model) with its `size` set to
   `g_btlArenaEffectExtentsCur`; the sky billboards' alpha drops by 0.1 (to 0 at or below 0).
   State 2 (holding): advances `g_btlMapFlashTimer`; once it was already 0x263 or the finish task
   0x14a exists, the state becomes 3 (timer 0x263). While no control lock or demo is active and the
   timer is below 0x1c3, every 50 frames (30 for kind 2, 150 when a NetPlay manager exists and
   profile flag 0 is set) it spawns `g_btlMapFlashAmbientEffect` at a ground point
   (`CollisionRaycastPoint` from 500 above) within ±300 of a random fighter other than the HUD's
   gate cut-in unit. For kind 1 and a timer below 0x227 it spawns effect 0x60 twice at the player's
   Bakugan and effect 0x61 on the ground below a point 800 above it and 200 along the camera
   direction, randomised by ±200 (`vrndf1`, `PlatformRandFloat12` - 1). State 3 (fading out): level -= 1/15; on reaching 0
   the state becomes 0, the flash material callback of every arena model is removed
   (`BtlStageModelSetFlash`) and `g_gfxFogParams` goes back to the arena fog
   (`BtlStageGetFogParams`); the sky alpha rises by 0.1 (max 1). While fading (state 1, or state 3
   not yet at 0) it blends the arena fog towards `g_btlMapFlashFogParams` by `(1 - cos(level *
   pi)) / 2` into `g_btlMapFlashFog` (`BtlStageLerpFog`), points `g_gfxFogParams` at it and
   copies `BtlStageGetClearColor` into the display clear colour. Other states do nothing. */

void BtlStageUpdateAmbientEffects(void)

{
  float pos[4];
  float offset[3];
  float spot[4];
  union {
    float f;
    s32 i;
  } alpha;
  u8 fading;
  s32 state;
  s32 timer;
  s32 period;
  s32 i;
  GfxEffect *effect;
  BtlBakugan *unit;
  GfxDisplay *display;
  float *color;
  float x;
  float z;
  float r;
  float c;

  fading = 0;
  if (g_btlSkySprites[0] != NULL) {
    if (CoreTaskExists(0x1e1) != 0) {
      g_btlSkySprites[0]->flags &= ~1u;
      g_btlSkySprites[1]->flags &= ~1u;
    } else {
      g_btlSkySprites[0]->flags |= 1u;
      g_btlSkySprites[1]->flags |= 1u;
    }
  }
  if (g_btlMapFlashState == 0) {
    return;
  }
  state = g_btlMapFlashState;
  if (state < 2) {
    if (state <= 0) {
      return;
    }
    g_btlMapFlashLevel = g_btlMapFlashLevel + 0.0666666701f;
    if (!(g_btlMapFlashLevel < 1.0f)) {
      g_btlMapFlashLevel = 1.0f;
      g_btlMapFlashState = 2;
      effect = GfxEffectSpawnOwned(g_worldEffectMgr, g_btlMapFlashLoopEffect[g_btlMapFlashKind],
                                   (const float *)&g_gfxVecZero, g_btlArenaModels[0]);
      for (i = 0; i < 4; i++) {
        effect->size[i] = g_btlArenaEffectExtentsCur[i];
      }
    }
    if (g_btlSkySprites[0] != NULL) {
      alpha.f = g_btlSkySprites[0]->alpha - 0.1f;
      /* sign test on the raw bits (mfc1 + bgtz): 0, -0 and negatives become 0 */
      if (alpha.i <= 0) {
        alpha.f = 0.0f;
      }
      g_btlSkySprites[0]->alpha = alpha.f;
      g_btlSkySprites[1]->alpha = g_btlSkySprites[0]->alpha;
    }
    fading = 1;
  } else if (state < 3) {
    timer = g_btlMapFlashTimer;
    g_btlMapFlashTimer = timer + 1;
    if (timer >= 0x263 || CoreTaskExists(0x14a) != 0) {
      g_btlMapFlashState = 3;
      g_btlMapFlashTimer = 0x263;
    }
    if (g_btlControlLockAll == 0 && !BtlIsDemoRunning() && g_btlMapFlashTimer < 0x1c3) {
      period = 50;
      if (g_btlMapFlashKind == 2) {
        period = 30;
      }
      if (NetPlayHasManager() && SaveGetProfileFlag0() != 0) {
        period = 150;
      }
      if (g_btlMapFlashTimer % period == 0) {
        unit = BtlPickRandomOtherFighter(((BtlHud *)UiGetTalkTask())->gateCutInUnit);
        if (unit != NULL) {
          for (i = 0; i < 4; i++) {
            pos[i] = unit->base.pos[i];
          }
          pos[1] = pos[1] + 500.0f;
          if (CollisionRaycastPoint(pos, pos) != 0) {
            x = pos[0];
            r = CoreRandFloat(600.0f);
            z = pos[2];
            pos[0] = x + (r - 300.0f);
            r = CoreRandFloat(600.0f);
            pos[2] = z + (r - 300.0f);
            GfxEffectSpawn(g_worldEffectMgr, g_btlMapFlashAmbientEffect[g_btlMapFlashKind], pos);
          }
        }
      }
    }
    if (g_btlMapFlashTimer < 0x227 && g_btlMapFlashKind == 1) {
      unit = BtlGetPlayerBakugan();
      if (unit != NULL) {
        GfxEffectSpawn(g_worldEffectMgr, 0x60, unit->base.pos);
        GfxEffectSpawn(g_worldEffectMgr, 0x60, unit->base.pos);
        for (i = 0; i < 4; i++) {
          spot[i] = unit->base.pos[i];
        }
        spot[1] = spot[1] + 800.0f;
        /* offset = camera dir * 200 (lanes x..z; the stored w lane is never read) */
        for (i = 0; i < 3; i++) {
          offset[i] = g_gfxActiveCamera->dir[i] * 200.0f;
        }
        for (i = 0; i < 3; i++) {
          spot[i] = spot[i] + offset[i];
        }
        x = spot[0];
        r = PlatformRandFloat12() - 1.0f;
        spot[0] = x + (r * 400.0f - 200.0f);
        z = spot[2];
        r = PlatformRandFloat12() - 1.0f;
        spot[2] = z + (r * 400.0f - 200.0f);
        if (CollisionRaycastPoint(spot, spot) != 0) {
          GfxEffectSpawn(g_worldEffectMgr, 0x61, spot);
        }
      }
    }
  } else if (state < 4) {
    g_btlMapFlashLevel = g_btlMapFlashLevel - 0.0666666701f;
    if (g_btlMapFlashLevel <= 0.0f) {
      g_btlMapFlashState = 0;
      g_btlMapFlashLevel = 0.0f;
      for (i = 0; i < 3; i++) {
        if (g_btlArenaModels[i] != NULL) {
          BtlStageModelSetFlash(g_btlArenaModels[i], NULL, 0);
        }
      }
      g_gfxFogParams = BtlStageGetFogParams();
    } else {
      fading = 1;
    }
    if (g_btlSkySprites[0] != NULL) {
      alpha.f = g_btlSkySprites[0]->alpha + 0.1f;
      if (!(alpha.f <= 1.0f)) {
        alpha.f = 1.0f;
      }
      g_btlSkySprites[0]->alpha = alpha.f;
      g_btlSkySprites[1]->alpha = g_btlSkySprites[0]->alpha;
    }
  }
  if (fading) {
    /* vcos.s of (level * pi) * 2/pi quarter turns = cos(level * pi) */
    c = __builtin_cosf(g_btlMapFlashLevel * 3.14159274f);
    BtlStageLerpFog((1.0f - c) * 0.5f, &g_btlMapFlashFog, g_gfxFogColor,
                    &g_btlArenaFogParams[g_btlArenaIndex],
                    &g_btlMapFlashFogParams[g_btlMapFlashKind]);
    display = g_gfxDisplay;
    color = BtlStageGetClearColor();
    for (i = 0; i < 4; i++) {
      display->clearColor[i] = color[i];
    }
    g_gfxFogParams = &g_btlMapFlashFog;
  }
}

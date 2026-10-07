// bdc 0x088e3754 ActorPlayerStateHoldRFocusPoint
#include "bdc.h"

/* State 11 of the player actor (edit-man, `ActorPlayerCtor`) (pointer-to-member table
   `0x08a98b3c`), entered from idle while the R trigger is held (`ActorPlayerWantsGauntletView`,
   command bit `0x20000000`): every frame copies the GMO root matrix to `modelMtx` and raises its y
   by 1.2, as the anchor of its effects. With `waitTimer` 0 and the field HUD task 0xbb9 present
   (`CoreTaskFind`) it switches the camera to mode 9, a spring view along the player →
   `focusPoint` direction (`GameFieldCameraBeginMode9`, given a copy of `focusPoint`), fades the
   field HUD out (`UiFieldHudFaderFadeOut`), stops the player (velocity = 0, the VFPU bank zero
   C720), plays idle, spawns effects 0x4e/0x4f/0x50 following `modelMtx` into `holdEffects`
   (`GfxEffectSpawnFollowMatrix`) and sets `waitTimer` to 1; with `waitTimer` 1 it keeps turning
   toward `focusPoint` (`ActorTurnToward`, rate 1, max 8° per frame); other values do neither.
   Then, when R (pad button `0x200`) is not held, it blends the camera back to the follow view
   (`GameFieldCameraBeginBlendToFollow`) and returns to idle (`ActorSetState`). */

void ActorPlayerStateHoldRFocusPoint(ActorPlayer *self)
{
  UiFieldHud *hud;
  float *mtx;
  float *src;
  float focusCopy[4];
  s32 timer;
  s32 i;

  mtx = self->modelMtx;
  src = self->base.base.data->rootMatrix;
  for (i = 0; i < 16; i++) {
    mtx[i] = src[i];
  }
  self->modelMtx[13] = self->modelMtx[13] + 1.2f;
  timer = self->base.waitTimer;
  if (timer > 0) {
    if (timer < 2) {
      ActorTurnToward(atan2f(self->focusPoint[2] - self->base.base.pos[2],
                             self->focusPoint[0] - self->base.base.pos[0]),
                      1.0f, 0.13962634f, self);
    }
  }
  else if (timer == 0) {
    hud = (UiFieldHud *)CoreTaskFind(0xbb9);
    if (hud != NULL) {
      for (i = 0; i < 4; i++) {
        focusCopy[i] = self->focusPoint[i];
      }
      GameFieldCameraBeginMode9(self->base.camera, focusCopy);
      UiFieldHudFaderFadeOut(&hud->fader);
      for (i = 0; i < 4; i++) {
        self->base.base.velocity[i] = 0.0f;
      }
      ActorPlayMotion(0.2f, self, 0, 1, 0);
      self->holdEffects[0] = GfxEffectSpawnFollowMatrix(g_worldEffectMgr, 0x4e, mtx);
      self->holdEffects[1] = GfxEffectSpawnFollowMatrix(g_worldEffectMgr, 0x4f, mtx);
      self->holdEffects[2] = GfxEffectSpawnFollowMatrix(g_worldEffectMgr, 0x50, mtx);
      self->base.waitTimer = 1;
    }
  }
  if ((g_padState->buttons & 0x200) == 0) {
    GameFieldCameraBeginBlendToFollow(self->base.camera);
    ActorSetState(&self->base, 0, 0);
  }
}

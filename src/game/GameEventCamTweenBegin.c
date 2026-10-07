// bdc 0x088f2a8c GameEventCamTweenBegin
#include "bdc.h"

/* Start method (vtable `0x08af430c` slot 2, `+0x14`; class built by `GameEventCamTweenCtor`) of an event
   camera tween: according to the tween kind (`kind`: 0 eye, 1 target, 2 field of view) it captures
   the current camera value (`g_gfxActiveCamera`: `eye`, `target`, both scaled by 0.05f to 20.12
   fixed point rounded half away from zero; `fov` truncated to an integer) as the start and current
   value in the shared camera key record `keys`. Any other kind does nothing. */

static inline __attribute__((always_inline)) void GameEventCamTweenBeginFix(s32 *fx, const float *src)
{
  float v[3];
  s32 k;

  v[0] = src[0] * 0.05f;
  v[1] = src[1] * 0.05f;
  v[2] = src[2] * 0.05f;
  for (k = 0; k < 3; k++) {
    if (v[k] <= 0.0f) {
      fx[k] = (s32)(v[k] * 4096.0f - 0.5f);
    } else {
      fx[k] = (s32)(v[k] * 4096.0f + 0.5f);
    }
  }
}

void GameEventCamTweenBegin(GameEventCamTween *self)
{
  GfxCamera *cam = g_gfxActiveCamera;
  GameEventCamKeys *keys;
  s32 fx[3];
  u16 fov;

  if (self->kind == 0) {
    GameEventCamTweenBeginFix(fx, cam->eye);
    keys = self->keys;
    keys->eyeStart[0] = fx[0];
    keys->eyeStart[1] = fx[1];
    keys->eyeStart[2] = fx[2];
    keys->eye[0] = fx[0];
    keys->eye[1] = fx[1];
    keys->eye[2] = fx[2];
  } else if (self->kind == 1) {
    GameEventCamTweenBeginFix(fx, cam->target);
    keys = self->keys;
    keys->targetStart[0] = fx[0];
    keys->targetStart[1] = fx[1];
    keys->targetStart[2] = fx[2];
    keys->target[0] = fx[0];
    keys->target[1] = fx[1];
    keys->target[2] = fx[2];
  } else if (self->kind == 2) {
    fov = (u16)(s32)cam->fov;
    self->keys->fovStart = fov;
    self->keys->fov = fov;
  }
}

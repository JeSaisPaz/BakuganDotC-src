// bdc 0x088f2f94 GameEventCamTweenUpdate
#include "bdc.h"

/* Per-frame update (vtable `0x08af430c` slot 4, `+0x24`) of an event camera tween: increments
   `frame`, then by `kind` linearly interpolates one camera value of the key record `keys` from its
   start to its end key by `frame / frames` (20.12 fixed point; the 64-bit products are libgcc
   `__muldi3` calls, written as `s64` multiplies): kind 0 the eye, kind 1 the target (each
   written to `g_gfxActiveCamera` as floats `/ 4096 * 20` through the VFPU), kind 2 the fov
   (written to the camera's `fov` as a float); other kinds only count. Returns true once `frame >=
   frames`. The camera vector's `w` lane is written with a stale VFPU value (S713, never set
   here: `vscl.t` writes only xyz) and is left out of the C. */

bool GameEventCamTweenUpdate(GameEventCamTween *self)
{
  float v[3];
  GameEventCamKeys *keys;
  s32 t;
  s32 d0;
  s32 d1;
  s32 d2;
  s32 start0;

  self->frame = self->frame + 1;
  switch (self->kind) {
  case 0:
    keys = self->keys;
    start0 = keys->eyeStart[0];
    d0 = keys->eyeEnd[0] - start0;
    d1 = keys->eyeEnd[1] - keys->eyeStart[1];
    d2 = keys->eyeEnd[2] - keys->eyeStart[2];
    t = ((s32)self->frame << 12) / (s32)self->frames;
    keys->eye[0] = (s32)(((s64)t * d0) >> 12) + start0;
    keys->eye[1] = (s32)(((s64)t * d1) >> 12) + keys->eyeStart[1];
    keys->eye[2] = (s32)(((s64)t * d2) >> 12) + keys->eyeStart[2];
    {
      GfxCamera *cam = g_gfxActiveCamera;

      keys = self->keys;
      v[0] = (float)keys->eye[0] * 0.000244140625f;
      v[1] = (float)keys->eye[1] * 0.000244140625f;
      v[2] = (float)keys->eye[2] * 0.000244140625f;
      cam->eye[0] = v[0] * 20.0f;
      cam->eye[1] = v[1] * 20.0f;
      cam->eye[2] = v[2] * 20.0f;
    }
    break;
  case 1:
    keys = self->keys;
    start0 = keys->targetStart[0];
    d0 = keys->targetEnd[0] - start0;
    d1 = keys->targetEnd[1] - keys->targetStart[1];
    d2 = keys->targetEnd[2] - keys->targetStart[2];
    t = ((s32)self->frame << 12) / (s32)self->frames;
    keys->target[0] = (s32)(((s64)t * d0) >> 12) + start0;
    keys->target[1] = (s32)(((s64)t * d1) >> 12) + keys->targetStart[1];
    keys->target[2] = (s32)(((s64)t * d2) >> 12) + keys->targetStart[2];
    {
      GfxCamera *cam = g_gfxActiveCamera;

      keys = self->keys;
      v[0] = (float)keys->target[0] * 0.000244140625f;
      v[1] = (float)keys->target[1] * 0.000244140625f;
      v[2] = (float)keys->target[2] * 0.000244140625f;
      cam->target[0] = v[0] * 20.0f;
      cam->target[1] = v[1] * 20.0f;
      cam->target[2] = v[2] * 20.0f;
    }
    break;
  case 2:
    keys = self->keys;
    keys->fov = keys->fovStart +
                (u16)((((s32)keys->fovEnd - (s32)keys->fovStart) * (s32)self->frame) /
                      (s32)self->frames);
    g_gfxActiveCamera->fov = (float)(s32)self->keys->fov;
    break;
  default:
    break;
  }
  return !((s32)self->frame < (s32)self->frames);
}

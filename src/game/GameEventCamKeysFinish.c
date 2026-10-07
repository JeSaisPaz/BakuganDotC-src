// bdc 0x088f2d70 GameEventCamKeysFinish
#include "bdc.h"

/* Jumps camera key `kind` (0 eye, 1 target, 2 fov) to its end value (copies the end key into the
   start and current keys) and, when `apply`, writes it to the camera `g_gfxActiveCamera`: eye or
   target as floats `key / 4096 * 20` (the fourth word of the stored quad is a stale VFPU lane and is
   left out), fov as a float. Other kinds do nothing. */

static inline __attribute__((always_inline)) void GameEventCamKeysFinishStore(float *dst, const s32 *key)
{
  float x = (float)key[0] * 0.00024414062f;
  float y = (float)key[1] * 0.00024414062f;
  float z = (float)key[2] * 0.00024414062f;

  dst[0] = x * 20.0f;
  dst[1] = y * 20.0f;
  dst[2] = z * 20.0f;
}

void GameEventCamKeysFinish(GameEventCamKeys *self, s32 kind, u8 apply)
{
  switch (kind) {
  case 0:
    self->eyeStart[0] = self->eyeEnd[0];
    self->eyeStart[1] = self->eyeEnd[1];
    self->eyeStart[2] = self->eyeEnd[2];
    self->eye[0] = self->eyeEnd[0];
    self->eye[1] = self->eyeEnd[1];
    self->eye[2] = self->eyeEnd[2];
    break;
  case 1:
    self->targetStart[0] = self->targetEnd[0];
    self->targetStart[1] = self->targetEnd[1];
    self->targetStart[2] = self->targetEnd[2];
    self->target[0] = self->targetEnd[0];
    self->target[1] = self->targetEnd[1];
    self->target[2] = self->targetEnd[2];
    break;
  case 2:
    self->fovStart = self->fovEnd;
    self->fov = self->fovEnd;
    break;
  }
  if (apply == 0) {
    return;
  }
  switch (kind) {
  case 0:
    GameEventCamKeysFinishStore(g_gfxActiveCamera->eye, self->eye);
    break;
  case 1:
    GameEventCamKeysFinishStore(g_gfxActiveCamera->target, self->target);
    break;
  case 2:
    g_gfxActiveCamera->fov = (float)self->fov;
    break;
  }
}

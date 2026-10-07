// bdc 0x088ec0b0 GameEventCaptureCamera
#include "bdc.h"

/* Copies the active camera (`g_gfxActiveCamera`: eye, target, fov) into the event's camera key
   record (`GameEventCamKeys` `self->camKeys`): eye and target are scaled by 1/20 (0.05f, VFPU
   `vscl.t`) and stored as 20.12 fixed point (rounded half away from zero) into all four key slots
   (`*Orig`, `*Start`, `*End` and the current value), and the fov is truncated to an integer and
   stored into `fovOrig`, `fovStart`, `fovEnd` and `fov`. */

void GameEventCaptureCamera(GameEvent *self)
{
  float v[3];
  s32 fx[3];
  GfxCamera *cam;
  GameEventCamKeys *keys;
  u16 fov;
  int k;

  cam = g_gfxActiveCamera;

  /* v = cam->eye * 0.05f (vscl.t: xyz only) */
  v[0] = cam->eye[0] * 0.05f;
  v[1] = cam->eye[1] * 0.05f;
  v[2] = cam->eye[2] * 0.05f;
  for (k = 0; k < 3; k++) {
    if (v[k] <= 0.0f) {
      fx[k] = (s32)(v[k] * 4096.0f - 0.5f);
    } else {
      fx[k] = (s32)(v[k] * 4096.0f + 0.5f);
    }
  }
  keys = self->camKeys;
  keys->eyeOrig[0] = fx[0];
  keys->eyeOrig[1] = fx[1];
  keys->eyeOrig[2] = fx[2];
  self->camKeys->eyeStart[0] = fx[0];
  self->camKeys->eyeStart[1] = fx[1];
  self->camKeys->eyeStart[2] = fx[2];
  self->camKeys->eyeEnd[0] = fx[0];
  self->camKeys->eyeEnd[1] = fx[1];
  self->camKeys->eyeEnd[2] = fx[2];
  keys->eye[0] = fx[0];
  keys->eye[1] = fx[1];
  keys->eye[2] = fx[2];

  /* v = cam->target * 0.05f (vscl.t: xyz only) */
  v[0] = cam->target[0] * 0.05f;
  v[1] = cam->target[1] * 0.05f;
  v[2] = cam->target[2] * 0.05f;
  for (k = 0; k < 3; k++) {
    if (v[k] <= 0.0f) {
      fx[k] = (s32)(v[k] * 4096.0f - 0.5f);
    } else {
      fx[k] = (s32)(v[k] * 4096.0f + 0.5f);
    }
  }
  keys = self->camKeys;
  keys->targetOrig[0] = fx[0];
  keys->targetOrig[1] = fx[1];
  keys->targetOrig[2] = fx[2];
  self->camKeys->targetStart[0] = fx[0];
  self->camKeys->targetStart[1] = fx[1];
  self->camKeys->targetStart[2] = fx[2];
  self->camKeys->targetEnd[0] = fx[0];
  self->camKeys->targetEnd[1] = fx[1];
  self->camKeys->targetEnd[2] = fx[2];
  keys->target[0] = fx[0];
  keys->target[1] = fx[1];
  keys->target[2] = fx[2];

  fov = (u16)(s32)cam->fov;
  self->camKeys->fovOrig = fov;
  self->camKeys->fovStart = fov;
  self->camKeys->fovEnd = fov;
  self->camKeys->fov = fov;
}

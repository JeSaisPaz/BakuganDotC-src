// bdc 0x088c8a90 GameFieldCameraMode9Update
#include "bdc.h"

/* Per-frame step of the field camera's mode-9 helper (`holder` points to a
   `GameFieldCameraMode9State`): moves look-at and eye towards `lookTarget`/`eyeTarget` with a
   damped spring (stiffness `(omega / (2*damping))²`, velocity damping omega, dt 1/30; omega 10 and
   damping 0.8 are lazily initialised statics `g_gameFieldCameraMode9Omega` /
   `g_gameFieldCameraMode9Damping`), then runs the wall test `GameFieldCameraProbeCollide` from
   the new eye against the player position into `outEye`/`outLook`, and copies the spring's look-at
   to `outLook`. */

void GameFieldCameraMode9Update(void **holder, float *outEye, float *outLook)
{
  GameFieldCameraMode9State *s = (GameFieldCameraMode9State *)*holder;
  float playerPos[4];
  float accel[3];
  Actor *player;
  float k;
  float omega;
  int i;

  if (g_gameFieldCameraMode9DampingInit == 0) {
    g_gameFieldCameraMode9DampingInit = 1;
    g_gameFieldCameraMode9Damping = 0.8f;
  }
  if (g_gameFieldCameraMode9OmegaInit == 0) {
    g_gameFieldCameraMode9OmegaInit = 1;
    g_gameFieldCameraMode9Omega = 10.0f;
  }
  /* accel = (lookTarget - look) * k - lookVel * omega; lookVel += accel / 30; look += lookVel / 30
     (xyz only: the w words of lookVel and look are left as they are) */
  k = g_gameFieldCameraMode9Omega / (g_gameFieldCameraMode9Damping * 2.0f);
  k = k * k;
  omega = g_gameFieldCameraMode9Omega;
  for (i = 0; i < 3; i++) {
    accel[i] = VfSub(VfMul(VfSub(s->lookTarget[i], s->look[i]), k), VfMul(s->lookVel[i], omega));
    s->lookVel[i] = VfAdd(s->lookVel[i], VfMul(accel[i], 0.0333333351f));
  }
  for (i = 0; i < 3; i++) {
    s->look[i] = VfAdd(s->look[i], VfMul(s->lookVel[i], 0.0333333351f));
  }
  if (g_gameFieldCameraMode9DampingInit == 0) {
    g_gameFieldCameraMode9DampingInit = 1;
    g_gameFieldCameraMode9Damping = 0.8f;
  }
  if (g_gameFieldCameraMode9OmegaInit == 0) {
    g_gameFieldCameraMode9OmegaInit = 1;
    g_gameFieldCameraMode9Omega = 10.0f;
  }
  /* same step for the eye */
  k = g_gameFieldCameraMode9Omega / (g_gameFieldCameraMode9Damping * 2.0f);
  k = k * k;
  omega = g_gameFieldCameraMode9Omega;
  for (i = 0; i < 3; i++) {
    accel[i] = VfSub(VfMul(VfSub(s->eyeTarget[i], s->eye[i]), k), VfMul(s->eyeVel[i], omega));
    s->eyeVel[i] = VfAdd(s->eyeVel[i], VfMul(accel[i], 0.0333333351f));
  }
  for (i = 0; i < 3; i++) {
    s->eye[i] = VfAdd(s->eye[i], VfMul(s->eyeVel[i], 0.0333333351f));
  }
  player = (Actor *)ActorFindPlayer();
  for (i = 0; i < 4; i++) {
    playerPos[i] = player->base.pos[i];
  }
  GameFieldCameraProbeCollide(s->probe, outEye, outLook, s->eye, playerPos);
  for (i = 0; i < 4; i++) {
    outLook[i] = s->look[i];
  }
}

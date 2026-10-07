// bdc 0x088deb84 ActorUpdateDistanceFade
#include "bdc.h"

/* Distance culling/fade of an actor (vtable slot 5, `+0x2c`, shared by every actor class): marks the
   model visible, then
   - stage 1 (script global variable 1) with model id 7 or 0x1f, or the active camera
     (`g_gfxActiveCamera`, a `GameFieldCamera`) in mode 5: `*alpha = 1`, returns 0;
   - closer to the camera eye than `nearDist`: fades `*alpha` out by 0.2 (clamped at 0), capped at
     `1 - (nearDist - dist) / nearDist`; returns 0;
   otherwise the distance is the nearer of the camera eye and the player actor (`ActorFindPlayer`):
   - beyond `farDist`: hides the model, zeroes the alpha and returns 1;
   - beyond 50 units: projects `pos` (`GfxCameraProjectPointFacing`) and, when it is off screen
     (`GfxScreenPointIsVisible` with margin 64), hides the model, zeroes the alpha and returns 1;
   - otherwise fades in by 0.2 up to 1 within `0.9 * farDist`, or out by 0.2 down to 0 beyond it;
     returns 0. */

int ActorUpdateDistanceFade(GfxModel *self, float *pos, float *alpha, float nearDist, float farDist)
{
  const float *eye;
  float dx;
  float dy;
  float dz;
  float dist;
  float playerDist;
  float cap;
  float a;
  GfxModel *player;
  float screen[4];

  self->visible = 1;
  if (g_scriptGlobalVars[1] == 1 && (self->base.unk08 == 7 || self->base.unk08 == 0x1f)) {
    *alpha = 1.0f;
    return 0;
  }
  if (((GameFieldCamera *)g_gfxActiveCamera)->mode == 5) {
    *alpha = 1.0f;
    return 0;
  }
  /* |eye - pos| */
  eye = g_gfxActiveCamera->eye;
  dx = eye[0] - pos[0];
  dy = eye[1] - pos[1];
  dz = eye[2] - pos[2];
  dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);

  if (dist < nearDist) {
    cap = 1.0f - (nearDist - dist) / nearDist;
    a = *alpha - 0.2f;
    *alpha = a;
    if (a < 0.0f) {
      *alpha = 0.0f;
    } else if (!(*alpha <= cap)) {
      *alpha = cap;
    }
    return 0;
  }

  player = (GfxModel *)ActorFindPlayer();
  if (player != NULL) {
    /* |player pos - pos| */
    dx = player->pos[0] - pos[0];
    dy = player->pos[1] - pos[1];
    dz = player->pos[2] - pos[2];
    playerDist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (!(dist <= playerDist)) {
      dist = playerDist;
    }
  }
  if (farDist < dist) {
    self->visible = 0;
    *alpha = 0.0f;
    return 1;
  }
  if (!(dist <= 50.0f)) {
    GfxCameraProjectPointFacing(g_gfxActiveCamera, screen, pos);
    if (GfxScreenPointIsVisible(64.0f, screen) == 0) {
      self->visible = 0;
      *alpha = 0.0f;
      return 1;
    }
  }
  if (dist <= farDist * 0.9f) {
    a = *alpha + 0.2f;
    *alpha = a;
    if (!(a <= 1.0f)) {
      *alpha = 1.0f;
    }
  } else {
    a = *alpha - 0.2f;
    *alpha = a;
    if (a < 0.0f) {
      *alpha = 0.0f;
    }
  }
  return 0;
}

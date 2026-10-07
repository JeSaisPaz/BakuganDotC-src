// bdc 0x088d95cc GameGimmickDistanceFade
#include "bdc.h"

/* Distance fade shared by 15 model vtables (gimmicks and core points, slot 5); like
   `GfxModelDistanceFade` but with a 0.1 step and a camera-mode bypass. Marks the model visible,
   then
   - camera mode (`GameFieldCamera` `mode` of `g_gfxActiveCamera`) 5: sets `*alpha` to 1 and
     returns 0 (no distance test);
   - closer than `nearDist` to the camera eye: fades `*alpha` out by 0.1 (clamped at 0), capped at
     `1 - (nearDist - dist) / nearDist`; returns 0;
   - beyond `farDist`: hides the model, zeroes the alpha and returns 1;
   - beyond 50 units: projects `pos` (`GfxCameraProjectPointFacing`) and, when it is off screen
     (`GfxScreenPointIsVisible` with margin 64), hides the model, zeroes the alpha and returns 1;
   - otherwise fades in by 0.1 up to 1 within `0.9 * farDist`, or out by 0.1 down to 0 beyond it;
     returns 0.
   No direct callers (method pointer). */

s32 GameGimmickDistanceFade(float nearDist, float farDist, GfxModel *model, const float *pos, float *alpha)
{
  float dist;
  float cap;
  float a;
  float screen[4];

  model->visible = 1;
  if (((GameFieldCamera *)g_gfxActiveCamera)->mode == 5) {
    *alpha = 1.0f;
    return 0;
  }
  /* |eye - pos| over x, y, z */
  {
    const float *eye = g_gfxActiveCamera->eye;
    float dx = eye[0] - pos[0];
    float dy = eye[1] - pos[1];
    float dz = eye[2] - pos[2];

    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
  }

  if (dist < nearDist) {
    cap = 1.0f - (nearDist - dist) / nearDist;
    a = *alpha - 0.1f;
    *alpha = a;
    if (a < 0.0f) {
      *alpha = 0.0f;
    } else if (!(*alpha <= cap)) {
      *alpha = cap;
    }
    return 0;
  }
  if (farDist < dist) {
    model->visible = 0;
    *alpha = 0.0f;
    return 1;
  }
  if (!(dist <= 50.0f)) {
    GfxCameraProjectPointFacing(g_gfxActiveCamera, screen, pos);
    if (GfxScreenPointIsVisible(64.0f, screen) == 0) {
      model->visible = 0;
      *alpha = 0.0f;
      return 1;
    }
  }
  if (dist <= farDist * 0.9f) {
    a = *alpha + 0.1f;
    *alpha = a;
    if (!(a <= 1.0f)) {
      *alpha = 1.0f;
    }
  } else {
    a = *alpha - 0.1f;
    *alpha = a;
    if (a < 0.0f) {
      *alpha = 0.0f;
    }
  }
  return 0;
}

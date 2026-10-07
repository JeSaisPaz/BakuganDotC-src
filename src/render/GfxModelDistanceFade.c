// bdc 0x089de9f4 GfxModelDistanceFade
#include "bdc.h"

/* Distance-based visibility and fade of a model relative to the active camera's eye
   (`g_gfxActiveCamera`): marks the model visible, then
   - closer than `nearDist`: fades `*alpha` out by 0.2 (clamped at 0), capped at
     `1 - (nearDist - dist) / nearDist`; returns 0;
   - beyond `farDist`: hides the model, zeroes the alpha and returns 1;
   - beyond 50 units: projects `pos` (`GfxCameraProjectPointFacing`) and, when it is off screen
     (`GfxScreenPointIsVisible` with margin 64), hides the model, zeroes the alpha and returns 1;
   - otherwise fades in by 0.2 up to 1 within `0.9 * farDist`, or out by 0.2 down to 0 beyond it;
     returns 0.
   No direct callers (method pointer). */

s32 GfxModelDistanceFade(float nearDist, float farDist, GfxModel *self, const float *pos, float *alpha)
{
  float dx;
  float dy;
  float dz;
  float dist;
  float cap;
  float a;
  float screen[4] BDC_ALIGN16;

  self->visible = 1;
  /* |eye - pos| over x, y, z */
  dx = g_gfxActiveCamera->eye[0] - pos[0];
  dy = g_gfxActiveCamera->eye[1] - pos[1];
  dz = g_gfxActiveCamera->eye[2] - pos[2];
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

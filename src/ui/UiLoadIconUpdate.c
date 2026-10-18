// bdc 0x08808e28 UiLoadIconUpdate
#include "bdc.h"

/* Update of the 'now loading' icon task (vtable `0x08af145c` slot 2):
   spins the icon sprite by right-multiplying its matrix with a Z rotation of `-0.5 × spinSpeed`
   radians (twice when the display runs at frame skip 1; VFPU `vrot`/`vmmul` on the angle times the
   bank's 2/π), accelerates `spinSpeed` by 0.01 clamped to [-0.8, 0.8], and fades the sprite alpha in
   by 0.1 per frame (up to 1) while `visible`, or out by 0.05 (down to 0) while hidden; once hidden and
   fully transparent it resets `spinSpeed` to 0 instead. */

/* matrix = matrix * rotZ(angle) (row-major, `vmmul.q E200, E100, E000`): in each row, columns 0
   and 1 mix, columns 2 and 3 are kept (identity rows). */
static void UiLoadIconRotate(float *matrix, float angle)
{
  float c = VfCosQuarter(VfMul(angle, 0.636619747f));
  float s = VfSinQuarter(VfMul(angle, 0.636619747f));
  int i;

  for (i = 0; i < 4; i++) {
    float a0 = matrix[4 * i];
    float a1 = matrix[4 * i + 1];
    float a2 = matrix[4 * i + 2];
    float a3 = matrix[4 * i + 3];
    matrix[4 * i] = VfDot4(a0, a1, a2, a3, c, -s, 0.0f, 0.0f);
    matrix[4 * i + 1] = VfDot4(a0, a1, a2, a3, s, c, 0.0f, 0.0f);
    matrix[4 * i + 2] = VfDot4(a0, a1, a2, a3, 0.0f, 0.0f, 1.0f, 0.0f);
    matrix[4 * i + 3] = VfDot4(a0, a1, a2, a3, 0.0f, 0.0f, 0.0f, 1.0f);
  }
}

void UiLoadIconUpdate(CoreTask *self)

{
  UiLoadIcon *icon = (UiLoadIcon *)self;
  float speed;
  float alpha;

  UiLoadIconRotate(icon->sprite->matrix, icon->spinSpeed * -0.5f);
  if (g_gfxDisplay->frameSkip == 1) {
    UiLoadIconRotate(icon->sprite->matrix, icon->spinSpeed * -0.5f);
  }
  speed = icon->spinSpeed + 0.01f;
  if (!(speed <= 0.8f)) {
    speed = 0.8f;
  }
  else if (speed < -0.8f) {
    speed = -0.8f;
  }
  icon->spinSpeed = speed;
  if (icon->visible != 0) {
    if (icon->sprite->alpha < 1.0f) {
      alpha = icon->sprite->alpha + 0.1f;
      if (!(alpha <= 1.0f)) {
        alpha = 1.0f;
      }
      icon->sprite->alpha = alpha;
    }
    return;
  }
  if (icon->sprite->alpha <= 0.0f) {
    icon->spinSpeed = 0.0f;
    return;
  }
  alpha = icon->sprite->alpha - 0.05f;
  if (alpha < 0.0f) {
    alpha = 0.0f;
  }
  icon->sprite->alpha = alpha;
}

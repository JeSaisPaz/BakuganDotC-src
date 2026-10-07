// bdc 0x0882fffc BtlHudFabSetAlpha
#include "bdc.h"

/* Sets the colour of `.fab` animation `index` of the HUD's animation set `fabs` to white with
   alpha `alpha` (`color` = {1, 1, 1, alpha}), when it exists. The quad is copied through a
   stack temp with lv.q/sv.q; no VFPU value is read by callers. */

void BtlHudFabSetAlpha(float alpha, BtlHud *self, s32 index)

{
  if (self->fabs[index] != (GfxFab *)0x0) {
    float *dst = self->fabs[index]->color;
    dst[0] = 1.0f;
    dst[1] = 1.0f;
    dst[2] = 1.0f;
    dst[3] = alpha;
  }
  return;
}

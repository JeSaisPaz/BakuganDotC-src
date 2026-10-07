// bdc 0x0881d388 GfxEffectClamp01
#include "bdc.h"

/* Saturates `v` to [0, 1] with the VFPU bank constants (`vmin.s` against S733 = 1.0f, then `vmax.s`
   against S713 = 0.0f) and returns the result. Helper of `GfxEffectRunCommands`. */

float GfxEffectClamp01(float v)
{
  /* NaN: +NaN loses vmin and gives 1, -NaN wins vmin and then loses vmax, giving 0 (semantics table:
     vmin/vmax). A tie on -0.0f vs S713's +0.0f gives the second operand, +0.0f. */
  if (v != v)
    return __builtin_signbit(v) ? 0.0f : 1.0f;
  return v <= 0.0f ? 0.0f : v > 1.0f ? 1.0f : v;
}

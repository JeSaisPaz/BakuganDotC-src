// bdc 0x089de978 GfxModelSetFog
#include "bdc.h"

/* Enables fog for a model (`fogEnabled`): near distance `fogNear` = 0, far `fogFar` =
   `clamp(colour.w, 0, 1) * 0.005 - 0.005`, and the fog colour `fogColor` packed from `colour`
   (each lane saturated, scaled by 255, floored; `x` in the low byte).
   `GfxModelDlWriteState` emits these. */

void GfxModelSetFog(GfxModel *self, const ScePspFVector4 *colour)

{
  float t;

  self->fogEnabled = 1;
  self->fogNear = 0.0f;
  t = colour->w;
  /* vmin.s against S733 (1.0f), then vmax.s against S713 (0.0f): +NaN loses vmin and gives 1,
     -NaN wins vmin and then loses vmax, giving 0 (VfpuLift semantics table: vmin/vmax). */
  if (t != t)
    t = __builtin_signbit(t) ? 0.0f : 1.0f;
  else
    t = t <= 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
  self->fogFar = t * 0.005f + -0.005f;
  /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q */
  self->fogColor = (u32)VfI2uc(VfF2iz(VfSat0(colour->x) * 255.0f, 23)) |
                   (u32)VfI2uc(VfF2iz(VfSat0(colour->y) * 255.0f, 23)) << 8 |
                   (u32)VfI2uc(VfF2iz(VfSat0(colour->z) * 255.0f, 23)) << 16 |
                   (u32)VfI2uc(VfF2iz(VfSat0(colour->w) * 255.0f, 23)) << 24;
}

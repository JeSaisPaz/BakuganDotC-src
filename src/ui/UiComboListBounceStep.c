// bdc 0x089b2d34 UiComboListBounceStep
#include "bdc.h"

/* One frame of the damped bounce used by `UiComboList` for its zoom-in:
   increments `*frame`, multiplies `*amplitude` by 0.6 and sets `*scale = 1 - cos(frame * 0.53) *
   amplitude` (VFPU `vcos`); once the amplitude is <= 0.006 it sets `*scale = 1.0` and returns true.
    */

bool UiComboListBounceStep(float *amplitude, s32 *frame, float *scale)
{
  float amp;
  float angle;

  *frame = *frame + 1;
  if (*amplitude <= 0.006f) {
    *scale = 1.0f;
    return true;
  }
  amp = *amplitude * 0.6f;
  *amplitude = amp;
  angle = (float)*frame * 0.53f;
  /* vmul by S703 (2/pi) then vcos.s (quarter turns): cosine of the angle in radians */
  *scale = 1.0f - __builtin_cosf(angle) * amp;
  return false;
}

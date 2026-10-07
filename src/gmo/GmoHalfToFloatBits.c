// bdc 0x089daf64 GmoHalfToFloatBits
#include "bdc.h"

/* Converts an IEEE half-precision value (low 16 bits of `half`) to a single-precision float:
   sign `<< 31`, exponent rebiased by `+0x70` (a zero magnitude keeps exponent 0; bnel skips the
   add), mantissa `<< 13`. Denormals are not handled. The bits are built in a GPR, stored to the
   stack and reloaded into $f0 as the float return. */

float GmoHalfToFloatBits(u32 half)
{
  union { u32 u; float f; } bits;
  u32 exp = (half >> 10) & 0x1f;

  if ((half & 0x7fff) != 0) {
    exp += 0x70;
  }
  bits.u = ((half & 0x3ff) << 13) | ((half & 0x8000) << 16) | (exp << 23);
  return bits.f;
}

// bdc 0x08a0f000 __unpack_f
#include "bdc.h"

/* libgcc `fp-bit.c` `__unpack_f`: single-precision version of `__unpack_d` (classifies
   zero/denormal/normal/inf/NaN, sets sign, unbiased exponent and the left-aligned 32-bit fraction
   at 0x0c of an `FpNumberF`). Used by `__extendsfdf2`. */

void __unpack_f(const float *src, FpNumberF *dst)

{
  u32 bits;
  u32 fpClass;
  s32 exp;
  u32 biased;
  u32 frac;

  bits = *(const u32 *)src;
  biased = bits >> 0x17 & 0xff;
  dst->sign = bits >> 0x1f;
  frac = bits & 0x7fffff;
  if (biased == 0) {
    fpClass = 2;
    if (frac != 0) {
      frac = frac << 7;
      exp = -0x7e;
      dst->normalExp = -0x7e;
      dst->fpClass = 3;
      if (frac < 0x40000000) {
        do {
          frac = frac << 1;
          exp = exp + -1;
        } while (frac < 0x40000000);
        dst->normalExp = exp;
      }
      dst->fraction = frac;
      return;
    }
  }
  else if (biased == 0xff) {
    if (frac != 0) {
      if ((bits & 0x100000) == 0) {
        dst->fpClass = 0;
      }
      else {
        dst->fpClass = 1;
      }
      dst->fraction = frac;
      return;
    }
    fpClass = 4;
  }
  else {
    dst->fraction = frac << 7 | 0x40000000;
    fpClass = 3;
    dst->normalExp = biased - 0x7f;
  }
  dst->fpClass = fpClass;
  return;
}

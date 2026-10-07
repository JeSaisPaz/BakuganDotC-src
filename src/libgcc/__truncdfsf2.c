// bdc 0x08a0ef88 __truncdfsf2
#include "bdc.h"

/* libgcc soft-float `__truncdfsf2`: narrows a double to float: `__unpack_d`, then shifts the
   64-bit fraction down to 30 bits (keeping a sticky bit) and calls `__make_fp`. */

float __truncdfsf2(double a)
{
  FpNumber u;
  double aa[2];
  u32 frac;

  aa[0] = a;
  __unpack_d(aa, &u);
  frac = (u32)(((u32)u.fraction & 0x3fffffff) != 0) | ((u32)u.fraction >> 0x1e) |
         ((u32)(u.fraction >> 32) << 2);
  return __make_fp(u.fpClass, u.sign, u.normalExp, frac);
}

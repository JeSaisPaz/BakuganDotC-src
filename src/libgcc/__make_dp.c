// bdc 0x08a0ef54 __make_dp
#include "bdc.h"

/* libgcc `fp-bit.c` helper `__make_dp`: assembles an `FpNumber` from its four fields on the stack
   and returns `__pack_d` of it. Used by `__extendsfdf2` to widen an unpacked float. */

double __make_dp(u32 fpClass, u32 sign, s32 exp, unsigned long long frac)
{
  FpNumber num;

  num.fpClass = fpClass;
  num.sign = sign;
  num.normalExp = exp;
  num.fraction = frac;
  return __pack_d(&num);
}

// bdc 0x08a0f0cc __make_fp
#include "bdc.h"

/* libgcc `fp-bit.c` helper `__make_fp`: builds a single-precision unpacked number on the stack and
   returns `__pack_f` of it. Used by `__truncdfsf2`. */

float __make_fp(u32 fpClass, u32 sign, s32 exp, u32 frac)
{
  FpNumberF num;

  num.fpClass = fpClass;
  num.sign = sign;
  num.normalExp = exp;
  num.fraction = frac;
  return __pack_f(&num);
}

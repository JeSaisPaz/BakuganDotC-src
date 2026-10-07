// bdc 0x08a0e2f8 __extendsfdf2
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `__extendsfdf2` at US
   `0x08a0e328`. */

double __extendsfdf2(float a)

{
  FpNumberF num;
  float arg[4];

  arg[0] = a;
  __unpack_f(arg, &num);
  return __make_dp(num.fpClass, num.sign, num.normalExp, (unsigned long long)num.fraction << 30);
}

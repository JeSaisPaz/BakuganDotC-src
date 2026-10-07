// bdc 0x08a0ef10 __negdf2
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `__negdf2` at US `0x08a0ef40`.
    */

double __negdf2(double a)

{
  FpNumber num;
  double arg[2];

  arg[0] = a;
  __unpack_d(arg, &num);
  num.sign = (u32)(num.sign == 0);
  return __pack_d(&num);
}

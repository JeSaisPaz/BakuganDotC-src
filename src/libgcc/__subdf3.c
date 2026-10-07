// bdc 0x08a0e6f4 __subdf3
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `__subdf3` at US `0x08a0e724`.
    */

double __subdf3(double a, double b)
{
  FpNumber ua;
  FpNumber ub;
  FpNumber tmp;
  double aa[2];
  double bb[2];

  aa[0] = a;
  bb[0] = b;
  __unpack_d(aa, &ua);
  __unpack_d(bb, &ub);
  ub.sign ^= 1;
  return __pack_d(_fpadd_parts(&ua, &ub, &tmp));
}

// bdc 0x08a0e68c __adddf3
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `__adddf3` at US `0x08a0e6bc`.
    */

double __adddf3(double a, double b)
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
  return __pack_d(_fpadd_parts(&ua, &ub, &tmp));
}

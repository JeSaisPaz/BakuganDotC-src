// bdc 0x08a0ed04 __cmpdf2
#include "bdc.h"

/* libgcc soft-float three-way compare (`fp-bit.c` `__cmpdf2`): unpacks both operands with
   `__unpack_d` and returns `__fpcmp_parts_d` (-1/0/1; 1 when either is NaN). Used by libm and
   printf for every double comparison (`huge + x > 0`, `x < 0.5`, ...). */

int __cmpdf2(double a, double b)

{
  FpNumber numA;
  FpNumber numB;
  double argA[2];
  double argB[2];

  argA[0] = a;
  argB[0] = b;
  __unpack_d(argA, &numA);
  __unpack_d(argB, &numB);
  return __fpcmp_parts_d(&numA, &numB);
}

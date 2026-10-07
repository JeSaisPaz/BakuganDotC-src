// bdc 0x08a0d1c4 __muldi3
#include "bdc.h"

/* libgcc `__muldi3`: 64-bit multiply, `a * b` modulo 2^64, from the 32-bit halves (`alo*blo`
   widened plus the cross products into the high word). */

long long __muldi3(long long a, long long b)

{
  return a * b;
}


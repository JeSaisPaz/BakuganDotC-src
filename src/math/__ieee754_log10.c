// bdc 0x089baa20 __ieee754_log10
#include "bdc.h"

/* The fdlibm core __ieee754_log10(x): splits x = 2^k * m and returns
   (k*log10_2lo + ivln10*__ieee754_log(m)) + k*log10_2hi, with the constants read from `g_ieeeLog10Consts`, `g_ieeeLogConsts` and
   `g_ieeeLogLiterals` (shared libm pools). x == +-0 gives -Inf (-two54/0),
   x < 0 gives NaN (0/0), infinities and NaNs return x + x. Subnormals are scaled by 2^54 first. */
double __ieee754_log10(double x)
{
  union { double d; struct { u32 lo; u32 hi; } w; } bits;
  s32 hx, k, i;
  double y, z;

  bits.d = x;
  hx = (s32)bits.w.hi;
  k = 0;
  if (hx < 0x00100000) { /* x < 2^-1022 */
    if (((hx & 0x7fffffff) | bits.w.lo) == 0) {
      return g_ieeeLogLiterals.negTwo54 / g_ieeeLogConsts.zero; /* log(+-0) = -inf */
    }
    if (hx < 0) {
      return g_ieeeLogConsts.zero / g_ieeeLogConsts.zero; /* log(-#) = NaN */
    }
    k = -54;
    x *= g_ieeeLogConsts.two54; /* scale up subnormal x */
    bits.d = x;
    hx = (s32)bits.w.hi;
  }
  if (hx >= 0x7ff00000) {
    return x + x;
  }
  k += (hx >> 20) - 1023;
  i = (s32)(((u32)k & 0x80000000U) >> 31);
  hx = (hx & 0x000fffff) | ((0x3ff - i) << 20);
  y = (double)(k + i);
  bits.w.hi = (u32)hx;
  z = y * g_ieeeLog10Consts.log10_2lo + g_ieeeLog10Consts.ivln10 * __ieee754_log(bits.d);
  return z + y * g_ieeeLog10Consts.log10_2hi;
}

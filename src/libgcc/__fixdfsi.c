// bdc 0x08a0ee3c __fixdfsi
#include "bdc.h"

/* libgcc soft-float `__fixdfsi`: truncates a double to a 32-bit signed integer via `__unpack_d`;
   NaN and zero give 0, negative exponents give 0, overflow and infinity saturate to
   `INT_MAX`/`INT_MIN`. Used by fdlibm's `(int)x == 0` tiny-argument checks (`__kernel_cos`,
   `__kernel_sin`, `__kernel_rem_pio2`) and printf. */

int __fixdfsi(double a)
{
  FpNumber u;
  double aa[2];
  u32 sh;
  u32 lo;
  u32 hi;
  u32 r;

  aa[0] = a;
  __unpack_d(aa, &u);
  if (u.fpClass == 2 || u.fpClass < 2) {
    return 0;
  }
  if (u.fpClass != 4) {
    if (u.normalExp < 0) {
      return 0;
    }
    if (u.normalExp < 0x1f) {
      sh = 0x3c - u.normalExp;
      lo = (u32)u.fraction;
      hi = (u32)(u.fraction >> 32);
      if ((s32)(sh << 26) < 0) {
        r = hi >> (sh & 0x1f);
      } else {
        r = lo >> (sh & 0x1f);
        if (sh << 26 != 0) {
          r |= hi << (-sh & 0x1f);
        }
      }
      if (u.sign != 0) {
        return -r;
      }
      return r;
    }
  }
  if (u.sign == 0) {
    return 0x7fffffff;
  }
  return -0x80000000;
}

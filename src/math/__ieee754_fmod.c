// bdc 0x089b9cf0 __ieee754_fmod
#include "bdc.h"

/* The fdlibm core __ieee754_fmod(x, y): the remainder of x / y with the sign of x, computed by
   shift-and-subtract on the exponent/mantissa words. y == 0, x infinite/NaN or y NaN give
   (x*y)/(x*y) (NaN); |x| < |y| returns x; |x| == |y| or an exact zero remainder gives a zero with
   the sign of x. */
double __ieee754_fmod(double x, double y)
{
  union { double d; struct { u32 lo; u32 hi; } w; } bits;
  s32 n, hx, hy, hz, ix, iy, sx, i;
  u32 lx, ly, lz;

  bits.d = x;
  hx = (s32)bits.w.hi;
  lx = bits.w.lo;
  bits.d = y;
  hy = (s32)bits.w.hi;
  ly = bits.w.lo;
  sx = hx & 0x80000000;
  hx ^= sx;          /* |x| */
  hy &= 0x7fffffff;  /* |y| */

  /* purge off exception values */
  if ((hy | (s32)ly) == 0 || hx >= 0x7ff00000 || ((u32)hy | ((ly | -ly) >> 31)) > 0x7ff00000U) {
    return (x * y) / (x * y);
  }
  if (hx <= hy) {
    if (hx < hy || lx < ly) {
      return x; /* |x| < |y| */
    }
    if (lx == ly) {
      return sx != 0 ? -0.0 : 0.0; /* |x| == |y| */
    }
  }

  /* ix = ilogb(x) */
  if (hx < 0x00100000) {
    if (hx == 0) {
      for (ix = -1043, i = (s32)lx; i > 0; i <<= 1) {
        ix -= 1;
      }
    } else {
      for (ix = -1022, i = hx << 11; i > 0; i <<= 1) {
        ix -= 1;
      }
    }
  } else {
    ix = (hx >> 20) - 1023;
  }
  /* iy = ilogb(y) */
  if (hy < 0x00100000) {
    if (hy == 0) {
      for (iy = -1043, i = (s32)ly; i > 0; i <<= 1) {
        iy -= 1;
      }
    } else {
      for (iy = -1022, i = hy << 11; i > 0; i <<= 1) {
        iy -= 1;
      }
    }
  } else {
    iy = (hy >> 20) - 1023;
  }

  /* align mantissas */
  if (ix >= -1022) {
    hx = 0x00100000 | (0x000fffff & hx);
  } else {
    n = -1022 - ix;
    if (n <= 31) {
      hx = (hx << n) | (s32)(lx >> (32 - n));
      lx <<= n;
    } else {
      hx = (s32)(lx << (n - 32));
      lx = 0;
    }
  }
  if (iy >= -1022) {
    hy = 0x00100000 | (0x000fffff & hy);
  } else {
    n = -1022 - iy;
    if (n <= 31) {
      hy = (hy << n) | (s32)(ly >> (32 - n));
      ly <<= n;
    } else {
      hy = (s32)(ly << (n - 32));
      ly = 0;
    }
  }

  /* fixed point fmod */
  n = ix - iy;
  while (n--) {
    hz = hx - hy;
    lz = lx - ly;
    if (lx < ly) {
      hz -= 1;
    }
    if (hz < 0) {
      hx = hx + hx + (s32)(lx >> 31);
      lx = lx + lx;
    } else {
      if ((hz | (s32)lz) == 0) {
        return sx != 0 ? -0.0 : 0.0;
      }
      hx = hz + hz + (s32)(lz >> 31);
      lx = lz + lz;
    }
  }
  hz = hx - hy;
  lz = lx - ly;
  if (lx < ly) {
    hz -= 1;
  }
  if (hz >= 0) {
    hx = hz;
    lx = lz;
  }

  /* convert back to floating value and restore the sign */
  if ((hx | (s32)lx) == 0) {
    return sx != 0 ? -0.0 : 0.0;
  }
  while (hx < 0x00100000) { /* normalise */
    hx = hx + hx + (s32)(lx >> 31);
    lx = lx + lx;
    iy -= 1;
  }
  if (iy >= -1022) {
    hx = (hx - 0x00100000) | ((iy + 1023) << 20);
  } else { /* subnormal output */
    n = -1022 - iy;
    if (n <= 20) {
      lx = (lx >> n) | ((u32)hx << (32 - n));
      hx >>= n;
    } else if (n <= 31) {
      lx = ((u32)hx << (32 - n)) | (lx >> n);
      hx = sx;
    } else {
      lx = (u32)(hx >> (n - 32));
      hx = sx;
    }
  }
  bits.w.hi = (u32)(hx | sx);
  bits.w.lo = lx;
  return bits.d;
}

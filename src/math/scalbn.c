// bdc 0x08a0cce4 scalbn
#include "bdc.h"

/* fdlibm `scalbn(x, n)`: `x * 2^n` computed by adjusting the exponent field directly.
   Zero, infinities and NaN come back unchanged (NaN/inf as `x + x`); subnormals are first
   normalised by `2^54`; overflow returns `huge * copysign(huge, x)`, underflow
   `tiny * copysign(tiny, x)`, and a subnormal result is built with an extra `2^54` and scaled
   down by `2^-54`. The double arithmetic is libgcc soft-float in the binary. */

#define TWO54 1.80143985094819840000e+16  /* 0x43500000 00000000 */
#define TWOM54 5.55111512312578270212e-17 /* 0x3C900000 00000000 */
#define HUGE 1.0e+300
#define TINY 1.0e-300

double scalbn(double x, int n)
{
    union {
        double d;
        u32 words[2]; /* little-endian: words[1] is the high word */
    } v;
    s32 hx;
    int k;

    v.d = x;
    hx = (s32)v.words[1];
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0) {
        /* 0 or subnormal */
        if ((v.words[0] | (hx & 0x7fffffff)) == 0) {
            return x; /* +-0 */
        }
        x = x * TWO54;
        v.d = x;
        hx = (s32)v.words[1];
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000) {
            return TINY * x; /* underflow */
        }
    }
    if (k == 0x7ff) {
        return x + x; /* NaN or Inf */
    }
    k = k + n;
    if (k > 0x7fe) {
        return HUGE * copysign(HUGE, x); /* overflow */
    }
    if (k > 0) {
        /* normal result */
        v.d = x;
        v.words[1] = ((u32)hx & 0x800fffff) | ((u32)k << 20);
        return v.d;
    }
    if (k <= -54) {
        if (n > 50000) {
            return HUGE * copysign(HUGE, x); /* overflow (n + k wrapped) */
        }
        return TINY * copysign(TINY, x); /* underflow */
    }
    /* subnormal result */
    k += 54;
    v.d = x;
    v.words[1] = ((u32)hx & 0x800fffff) | ((u32)k << 20);
    return v.d * TWOM54;
}

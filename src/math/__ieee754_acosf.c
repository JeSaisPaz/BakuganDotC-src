// bdc 0x08a08e64 __ieee754_acosf
#include "bdc.h"

/* fdlibm/newlib `__ieee754_acosf` (`ef_acos.c`): single-precision arc cosine: `|x| == 1` gives 0
   or pi, `|x| > 1` NaN (0/0), `|x| <= 2^-57` pi/2, `|x| < 0.5` the `pS`/`qS` rational
   approximation, larger values through `__ieee754_sqrtf``((1-|x|)/2)`. Worker behind
   `acosf`. */

#define PI_F      0x1.921fb6p+1f  /* 0x40490fdb */
#define PI_HI     0x1.921fb4p+1f  /* 0x40490fda */
#define PIO2_F    0x1.921fb6p+0f  /* 0x3fc90fdb */
#define PIO2_HI   0x1.921fb4p+0f  /* 0x3fc90fda */
#define PIO2_LO   0x1.4442d0p-24f /* 0x33a22168 */
#define PS0       0x1.555556p-3f
#define PS1      -0x1.4d6120p-2f
#define PS2       0x1.9c1550p-3f
#define PS3      -0x1.48228cp-5f
#define PS4       0x1.9efe08p-11f
#define PS5       0x1.23de10p-15f
#define QS1      -0x1.33a272p+1f
#define QS2       0x1.02ae5ap+1f
#define QS3      -0x1.6066c2p-1f
#define QS4       0x1.3b8c5cp-4f

/* z * (pS0 + z*(pS1 + ... + z*pS5)) */
static inline float AcosP(float z)
{
    return z * (PS0 + z * (PS1 + z * (PS2 + z * (PS3 + z * (PS4 + z * PS5)))));
}

/* 1 + z*(qS1 + ... + z*qS4) */
static inline float AcosQ(float z)
{
    return 1.0f + z * (QS1 + z * (QS2 + z * (QS3 + z * QS4)));
}

float __ieee754_acosf(float x)
{
    union {
        float f;
        s32 bits;
    } in, head;
    s32 hx;
    s32 ix;
    float z;
    float s;
    float r;
    float c;

    in.f = x;
    hx = in.bits;
    ix = hx & 0x7fffffff;

    if (ix == 0x3f800000) { /* |x| == 1 */
        return hx > 0 ? 0.0f : PI_F;
    }
    if (ix > 0x3f800000) { /* |x| > 1: NaN */
        z = 0.0f;
        return z / z;
    }
    if (ix < 0x3f000000) { /* |x| < 0.5 */
        if (ix <= 0x23000000) {
            return PIO2_F;
        }
        z = x * x;
        r = AcosP(z) / AcosQ(z);
        return PIO2_HI - (x - (PIO2_LO - r * x));
    }
    if (hx < 0) { /* x < -0.5 */
        z = (x + 1.0f) * 0.5f;
        s = __ieee754_sqrtf(z);
        r = AcosP(z) / AcosQ(z);
        return PI_HI - (s + (r * s - PIO2_LO)) * 2.0f;
    }
    /* x > 0.5 */
    z = (1.0f - x) * 0.5f;
    s = __ieee754_sqrtf(z);
    head.f = s;
    head.bits &= (s32)0xfffff000;
    c = (z - head.f * head.f) / (head.f + s);
    r = AcosP(z) / AcosQ(z);
    return (head.f + (r * s + c)) * 2.0f;
}

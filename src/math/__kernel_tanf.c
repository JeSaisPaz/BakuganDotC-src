// bdc 0x08a0c384 __kernel_tanf
#include "bdc.h"

/* fdlibm `__kernel_tanf`: tangent on `[-pi/4, pi/4]` of `x + y`; `k == 1` returns tan, `k == -1`
   returns `-1/tan`. Tiny `|x| < 2^-28` returns `x`, `1/|x|` or `-1/x` directly; `|x| >= 0.6744`
   folds through `pi/4 - x`; otherwise the `T0..T12` polynomial (`g_tanfT`). Called by `tanf`. */
float __kernel_tanf(float x, float y, int k)
{
    union { float f; s32 i; } bits;
    s32 hx, ix;
    float z, w, r, v, s, t, a;

    bits.f = x;
    hx = bits.i;
    ix = hx & 0x7fffffff;
    if (ix < 0x31800000 && (int)x == 0) {
        /* |x| < 2^-28 */
        if ((ix | (k + 1)) == 0) {
            return 1.0f / fabsf(x); /* x == 0 and k == -1 */
        }
        if (k == 1) {
            return x;
        }
        return -1.0f / x;
    }
    if (ix >= 0x3f2ca140) {
        /* |x| >= 0.6744: use tan(pi/4 - x) */
        if (hx < 0) {
            x = -x;
            y = -y;
        }
        z = 0.7853981f - x;      /* pio4 */
        w = 3.7748947e-08f - y;  /* pio4lo */
        x = w + z;
        y = 0.0f;
    }
    z = x * x;
    w = z * z;
    /* odd terms in r, even terms in v */
    r = g_tanfT[1] + w * (g_tanfT[3] + w * (g_tanfT[5] + w * (g_tanfT[7] + w * (g_tanfT[9] + w * g_tanfT[11]))));
    v = z * (g_tanfT[2] + w * (g_tanfT[4] + w * (g_tanfT[6] + w * (g_tanfT[8] + w * (g_tanfT[10] + w * g_tanfT[12])))));
    s = x * z;
    r = y + z * (s * (r + v) + y);
    r += g_tanfT[0] * s;
    w = r + x;
    if (ix >= 0x3f2ca140) {
        v = (float)k;
        return (float)(1 - ((hx >> 30) & 2)) * (v - 2.0f * (x - (w * w / (v + w) - r)));
    }
    if (k == 1) {
        return w;
    }
    /* -1/(x + r) computed with the high halves split off for accuracy */
    bits.f = w;
    bits.i &= (s32)0xfffff000;
    z = bits.f;
    v = r - (z - x);
    a = -1.0f / w;
    bits.f = a;
    bits.i &= (s32)0xfffff000;
    t = bits.f;
    s = t * z + 1.0f;
    return t + a * (s + t * v);
}

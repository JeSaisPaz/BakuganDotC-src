// bdc 0x08a0c8b8 atanf
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `atanf` at US `0x08a0c8e8`.
   fdlibm `atanf`: reduces |x| to one of five ranges (id -1..3: tiny, [7/16, 11/16), [11/16, 19/16),
   [19/16, 39/16), >= 39/16), evaluates the odd polynomial `g_atanfAT` and adds the range's base
   angle `g_atanfHi` + `g_atanfLo`, restoring the sign. |x| >= 2^34 returns +-pi/2; NaN returns
   `x + x`. */
float atanf(float x)
{
    union { float f; s32 i; } bits;
    s32 hx, ix;
    int id;
    float w, s1, s2, z;

    bits.f = x;
    hx = bits.i;
    ix = hx & 0x7fffffff;
    if (ix >= 0x50800000) {
        /* |x| >= 2^34 */
        if (ix > 0x7f800000) {
            return x + x; /* NaN */
        }
        if (hx > 0) {
            return g_atanfHi[3] + g_atanfLo[3];
        }
        return -g_atanfHi[3] - g_atanfLo[3];
    }
    if (ix < 0x3ee00000) {
        /* |x| < 7/16 */
        if (ix < 0x31000000) {
            /* |x| < 2^-29: raise inexact */
            if (!(x + 1e30f <= 1.0f)) {
                return x;
            }
        }
        id = -1;
    } else {
        x = fabsf(x);
        if (ix < 0x3f980000) {
            /* |x| < 1.1875 */
            if (ix < 0x3f300000) {
                /* 7/16 <= |x| < 11/16 */
                id = 0;
                x = (x * 2.0f - 1.0f) / (x + 2.0f);
            } else {
                /* 11/16 <= |x| < 19/16 */
                id = 1;
                x = (x - 1.0f) / (x + 1.0f);
            }
        } else if (ix < 0x401c0000) {
            /* |x| < 2.4375 */
            id = 2;
            x = (x - 1.5f) / (x * 1.5f + 1.0f);
        } else {
            /* 2.4375 <= |x| < 2^34 */
            id = 3;
            x = -1.0f / x;
        }
    }
    z = x * x;
    w = z * z;
    /* even and odd halves of the polynomial */
    s1 = z * (g_atanfAT[0] + w * (g_atanfAT[2] + w * (g_atanfAT[4] + w * (g_atanfAT[6] + w * (g_atanfAT[8] + w * g_atanfAT[10])))));
    s2 = w * (g_atanfAT[1] + w * (g_atanfAT[3] + w * (g_atanfAT[5] + w * (g_atanfAT[7] + w * g_atanfAT[9]))));
    if (id < 0) {
        return x - x * (s1 + s2);
    }
    z = g_atanfHi[id] - ((x * (s1 + s2) - g_atanfLo[id]) - x);
    return (hx < 0) ? -z : z;
}

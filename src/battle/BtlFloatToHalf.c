// bdc 0x08885dc8 BtlFloatToHalf
#include "bdc.h"

/* Converts an IEEE single to an IEEE half from its raw bits (stored to the stack and reloaded as
   a word: a bit reinterpretation): sign to bit 15; unbiased exponents below -14 give +-0; 16..127
   give +-infinity; exponent field 0xff (Inf/NaN) gives +-infinity with the low 10 mantissa bits
   kept (plus 0x400, already part of the exponent, when mantissa bit 18 is set); otherwise the
   exponent is rebiased and the top 10 mantissa bits are kept (truncating). Used to re-pack
   half-float motion keys after editing. */
u16 BtlFloatToHalf(float value)
{
    union {
        float f;
        s32 i;
    } bits;
    s32 exponent;
    s32 mantissa;
    u16 sign;

    bits.f = value;
    sign = (u16)(((bits.i >> 31) & 1) << 15);
    exponent = ((bits.i >> 23) & 0xff) - 0x7f;
    mantissa = bits.i & 0x7fffff;

    if (exponent < -14) {
        return sign;
    }
    if (exponent < 16) {
        return sign | (u16)(((exponent + 15) & 0x1f) << 10) | (u16)(mantissa >> 13);
    }
    if (exponent != 0x80 || mantissa == 0) {
        return sign | 0x7c00;
    }
    if (mantissa & 0x40000) {
        return sign | 0x7c00 | (u16)((mantissa & 0x3ff) | 0x400);
    }
    return sign | 0x7c00 | (u16)(mantissa & 0x3ff);
}

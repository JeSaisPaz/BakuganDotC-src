// bdc 0x08a0d13c copysign
#include "bdc.h"

/* fdlibm `copysign(double x, double y)`: `x` with its sign bit replaced by the sign bit of `y`
   (only the high words change). */
double copysign(double x, double y)
{
    union {
        double d;
        u32 words[2]; /* little-endian: words[1] is the high word */
    } vx, vy;

    vx.d = x;
    vy.d = y;
    vx.words[1] = (vx.words[1] & 0x7fffffffu) | (vy.words[1] & 0x80000000u);
    return vx.d;
}

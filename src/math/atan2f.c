// bdc 0x08a05570 atan2f
#include "bdc.h"

/* Standard newlib `atan2f` wrapper (`wf_atan2.c`): calls `__ieee754_atan2f` and, unless
   `g_libVersion` is `_IEEE_` (-1), reports `atan2f(0, 0)` (neither argument NaN) through a
   `MathException` (type DOMAIN, name "atan2f", args `y`, `x`, result
   `g_atan2fDomainRetval`): `_POSIX_` (2) sets errno to EDOM (33) directly, otherwise
   `matherr` is asked first. */
float atan2f(float y, float x)
{
    MathException exc;
    float result;

    result = __ieee754_atan2f(y, x);
    if (g_libVersion == -1 || isnanf(x) != 0 || isnanf(y) != 0) {
        return result;
    }
    if (!(x == 0.0f) || !(y == 0.0f)) {
        return result;
    }

    exc.arg1 = (double)y;
    exc.arg2 = (double)x;
    exc.err = 0;
    exc.type = 1; /* DOMAIN */
    exc.name = "atan2f";
    exc.retval = g_atan2fDomainRetval;
    if (g_libVersion == 2) {
        *__errno() = 33; /* EDOM */
    } else if (matherr(&exc) == 0) {
        *__errno() = 33; /* EDOM */
    }
    if (exc.err != 0) {
        *__errno() = exc.err;
    }
    return (float)exc.retval;
}

// bdc 0x08a05438 acosf
#include "bdc.h"

/* Standard newlib `acosf` wrapper (`wf_acos.c`): calls `__ieee754_acosf` and, unless
   `g_libVersion` is `_IEEE_` (-1), reports `|x| > 1` through a `MathException` (type DOMAIN,
   name "acosf", both arguments `x`, result `g_acosfDomainRetval`): `_POSIX_` (2) sets errno to
   EDOM (33) directly, otherwise `matherr` is asked first. Callers include actor movement/aiming
   (`ActorMoveWithCollision`, `ActorPlayerUpdateThrowAim`). */
float acosf(float x)
{
    MathException exc;
    float result;

    result = __ieee754_acosf(x);
    if (g_libVersion == -1 || isnanf(x) != 0 || fabsf(x) <= 1.0f) {
        return result;
    }

    exc.type = 1; /* DOMAIN */
    exc.name = "acosf";
    exc.err = 0;
    exc.arg1 = (double)x;
    exc.arg2 = exc.arg1;
    exc.retval = g_acosfDomainRetval;
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

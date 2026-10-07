// bdc 0x08a049b8 acos
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `acos` at US `0x08a049e8`.
   Standard newlib `acos` wrapper (`w_acos.c`): calls `__ieee754_acos` and, unless
   `g_libVersion` is `_IEEE_` (-1), reports `|x| > 1` through a `MathException` (type DOMAIN,
   name "acos", both arguments `x`, result 0.0 from rodata): `_POSIX_` (2) sets errno to EDOM (33)
   directly, otherwise `matherr` is asked first. */
double acos(double x)
{
    MathException exc;
    double result;

    result = __ieee754_acos(x);
    if (g_libVersion == -1 || isnan(x) != 0 || !(fabs(x) > 1.0)) {
        return result;
    }

    exc.type = 1; /* DOMAIN */
    exc.name = "acos";
    exc.err = 0;
    exc.retval = 0.0;
    exc.arg1 = x;
    exc.arg2 = x;
    if (g_libVersion == 2) {
        *__errno() = 33; /* EDOM */
    } else if (matherr(&exc) == 0) {
        *__errno() = 33; /* EDOM */
    }
    if (exc.err != 0) {
        *__errno() = exc.err;
    }
    return exc.retval;
}

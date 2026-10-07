// bdc 0x08a052a8 sqrt
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `sqrt` at US `0x08a052d8`.
   Standard newlib `sqrt` wrapper (`w_sqrt.c`): calls `__ieee754_sqrt` and, unless
   `g_libVersion` is `_IEEE_` (-1), reports `x < 0` through a `MathException` (type DOMAIN,
   name "sqrt", both arguments `x`; result 0 under `_SVID_`, else `0.0 / 0.0`): `_POSIX_` (2) sets
   errno to EDOM (33) directly, otherwise `matherr` is asked first. */
double sqrt(double x)
{
    MathException exc;
    double result;
    double zero = 0.0;

    result = __ieee754_sqrt(x);
    if (g_libVersion == -1 || isnan(x) != 0 || !(x < 0.0)) {
        return result;
    }

    exc.type = 1; /* DOMAIN */
    exc.name = "sqrt";
    exc.err = 0;
    exc.arg1 = x;
    exc.arg2 = x;
    if (g_libVersion == 0) {
        exc.retval = zero;
    } else {
        exc.retval = zero / zero;
    }
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

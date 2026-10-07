// bdc 0x08a04b18 pow
#include "bdc.h"

/* newlib `pow` SVID/XOPEN wrapper (`w_pow.c`): returns `__ieee754_pow``(x, y)` unless
   `g_libVersion` asks for error handling (anything but `_IEEE_` = -1) and the call hit one of
   the special cases below, each reported through a `MathException` named "pow" with
   `arg1 = x`, `arg2 = y`:
   - pow(NaN, 0): DOMAIN, result x, or 1.0 for `_IEEE_`/`_POSIX_` (2) without `matherr`;
   - pow(0, 0): DOMAIN, result 0.0 under `_SVID_` (0, after `matherr`), else 1.0;
   - pow(0, finite negative): DOMAIN, 0.0 (`_SVID_`) or -HUGE_VAL;
   - finite x, y with non-finite z: NaN z → DOMAIN, 0.0 (`_SVID_`) or 0.0/0.0; else OVERFLOW,
     +-HUGE (`_SVID_`) or +-HUGE_VAL, negative when x < 0 and y is an odd integer;
   - z == 0 with finite x, y: UNDERFLOW, 0.0.
   `_POSIX_` sets errno (EDOM 33 / ERANGE 34) directly, other modes only when matherr returns 0;
   a non-zero `exc.err` then overrides errno. Constants come from `g_powWrapperConsts` and
   `g_infinity`; double arithmetic and compares are the libgcc soft-float helpers
   (`__cmpdf2`, `__muldf3`, `__divdf3`, `__negdf2`) written as C operators. */
double pow(double x, double y)
{
    MathException exc;
    double z;

    z = __ieee754_pow(x, y);
    if (g_libVersion == -1 || isnan(y) != 0) {
        return z;
    }

    if (isnan(x) != 0) {
        if (y != g_powWrapperConsts.zero) {
            return z;
        }
        /* pow(NaN, 0.0) */
        exc.type = 1; /* DOMAIN */
        exc.name = "pow";
        exc.err = 0;
        exc.arg1 = x;
        exc.arg2 = y;
        exc.retval = x;
        if (g_libVersion == -1 || g_libVersion == 2) {
            exc.retval = g_powWrapperConsts.one;
        } else if (matherr(&exc) == 0) {
            *__errno() = 33; /* EDOM */
        }
        if (exc.err != 0) {
            *__errno() = exc.err;
        }
        return exc.retval;
    }

    if (x == g_powWrapperConsts.zero) {
        if (y == g_powWrapperConsts.zero) {
            /* pow(0.0, 0.0) */
            exc.type = 1; /* DOMAIN */
            exc.name = "pow";
            exc.err = 0;
            exc.arg1 = x;
            exc.arg2 = y;
            exc.retval = g_powWrapperConsts.zero;
            if (g_libVersion != 0) {
                exc.retval = g_powWrapperConsts.one;
            } else if (matherr(&exc) == 0) {
                *__errno() = 33; /* EDOM */
            }
            if (exc.err != 0) {
                *__errno() = exc.err;
            }
            return exc.retval;
        }
        if (finite(y) == 0 || !(y < g_powWrapperConsts.zero)) {
            return z;
        }
        /* 0 ** negative */
        exc.type = 1; /* DOMAIN */
        exc.name = "pow";
        exc.err = 0;
        exc.arg1 = x;
        exc.arg2 = y;
        if (g_libVersion == 0) {
            exc.retval = g_powWrapperConsts.zero;
        } else {
            exc.retval = -g_infinity;
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

    if (finite(z) == 0 && finite(x) != 0 && finite(y) != 0) {
        exc.name = "pow";
        exc.err = 0;
        exc.arg1 = x;
        exc.arg2 = y;
        if (isnan(z) != 0) {
            /* negative ** non-integer */
            exc.type = 1; /* DOMAIN */
            if (g_libVersion == 0) {
                exc.retval = g_powWrapperConsts.zero;
            } else {
                exc.retval = g_powWrapperConsts.zero / g_powWrapperConsts.zero;
            }
            if (g_libVersion == 2) {
                *__errno() = 33; /* EDOM */
            } else if (matherr(&exc) == 0) {
                *__errno() = 33; /* EDOM */
            }
        } else {
            /* overflow */
            exc.type = 3; /* OVERFLOW */
            y = y * g_powWrapperConsts.half;
            if (g_libVersion == 0) {
                exc.retval = g_powWrapperConsts.huge;
                if (x < g_powWrapperConsts.zero && rint(y) != y) {
                    exc.retval = g_powWrapperConsts.negHuge;
                }
            } else {
                exc.retval = g_infinity;
                if (x < g_powWrapperConsts.zero && rint(y) != y) {
                    exc.retval = -g_infinity;
                }
            }
            if (g_libVersion == 2) {
                *__errno() = 34; /* ERANGE */
            } else if (matherr(&exc) == 0) {
                *__errno() = 34; /* ERANGE */
            }
        }
        if (exc.err != 0) {
            *__errno() = exc.err;
        }
        return exc.retval;
    }

    if (z == g_powWrapperConsts.zero && finite(x) != 0 && finite(y) != 0) {
        /* underflow */
        exc.type = 4; /* UNDERFLOW */
        exc.name = "pow";
        exc.err = 0;
        exc.arg1 = x;
        exc.arg2 = y;
        exc.retval = g_powWrapperConsts.zero;
        if (g_libVersion == 2) {
            *__errno() = 34; /* ERANGE */
        } else if (matherr(&exc) == 0) {
            *__errno() = 34; /* ERANGE */
        }
        if (exc.err != 0) {
            *__errno() = exc.err;
        }
        return exc.retval;
    }
    return z;
}

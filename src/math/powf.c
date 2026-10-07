// bdc 0x08a056d4 powf
#include "bdc.h"

/* newlib `powf` SVID/XOPEN wrapper (`wf_pow.c`): returns `__ieee754_powf``(x, y)` unless
   `g_libVersion` asks for error handling (anything but `_IEEE_` = -1) and the call hit one of
   the special cases below, each reported through a `MathException` named "powf" with
   `arg1 = (double)x`, `arg2 = (double)y`:
   - powf(NaN, 0): DOMAIN, result x, or 1.0 for `_IEEE_`/`_POSIX_` (2) without `matherr`;
   - powf(0, 0): DOMAIN, result 0.0 under `_SVID_` (0, after `matherr`), else 1.0;
   - powf(0, finite negative): DOMAIN, 0.0 (`_SVID_`) or -HUGE_VAL;
   - finite x, y with non-finite z: NaN z → DOMAIN, 0.0 (`_SVID_`) or 0.0/0.0; else OVERFLOW,
     +-HUGE (`_SVID_`) or +-HUGE_VAL, negative when x < 0 and y is an odd integer (y is halved
     in float, the parity test is the double `rint`);
   - z == 0 with finite x, y: UNDERFLOW, 0.0.
   `_POSIX_` sets errno (EDOM 33 / ERANGE 34) directly, other modes only when matherr returns 0;
   a non-zero `exc.err` then overrides errno; the double `exc.retval` is narrowed to float.
   Constants come from `g_powfWrapperConsts` and `g_infinity`; double arithmetic, compares
   and conversions are the libgcc soft-float helpers (`__extendsfdf2`, `__truncdfsf2`,
   `__cmpdf2`, `__muldf3`, `__divdf3`, `__negdf2`) written as C operators and casts. */
float powf(float x, float y)
{
    MathException exc;
    float z;

    z = __ieee754_powf(x, y);
    if (g_libVersion == -1 || isnanf(y) != 0) {
        return z;
    }

    if (isnanf(x) != 0) {
        if (!(y == 0.0f)) {
            return z;
        }
        /* powf(NaN, 0.0) */
        exc.type = 1; /* DOMAIN */
        exc.name = "powf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        exc.retval = exc.arg1;
        if (g_libVersion == -1 || g_libVersion == 2) {
            exc.retval = g_powfWrapperConsts.one;
        } else if (matherr(&exc) == 0) {
            *__errno() = 33; /* EDOM */
        }
        if (exc.err != 0) {
            *__errno() = exc.err;
        }
        return (float)exc.retval;
    }

    if (x == 0.0f) {
        if (y == 0.0f) {
            /* powf(0.0, 0.0) */
            exc.type = 1; /* DOMAIN */
            exc.name = "powf";
            exc.err = 0;
            exc.arg1 = (double)x;
            exc.arg2 = (double)y;
            exc.retval = g_powfWrapperConsts.zero;
            if (g_libVersion != 0) {
                exc.retval = g_powfWrapperConsts.one;
            } else if (matherr(&exc) == 0) {
                *__errno() = 33; /* EDOM */
            }
            if (exc.err != 0) {
                *__errno() = exc.err;
            }
            return (float)exc.retval;
        }
        if (finitef(y) == 0 || !(y < 0.0f)) {
            return z;
        }
        /* 0 ** negative */
        exc.type = 1; /* DOMAIN */
        exc.name = "powf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        if (g_libVersion == 0) {
            exc.retval = g_powfWrapperConsts.zero;
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
        return (float)exc.retval;
    }

    if (finitef(z) == 0 && finitef(x) != 0 && finitef(y) != 0) {
        exc.name = "powf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        if (isnanf(z) != 0) {
            /* negative ** non-integer */
            exc.type = 1; /* DOMAIN */
            if (g_libVersion == 0) {
                exc.retval = g_powfWrapperConsts.zero;
            } else {
                exc.retval = g_powfWrapperConsts.zero / g_powfWrapperConsts.zero;
            }
            if (g_libVersion == 2) {
                *__errno() = 33; /* EDOM */
            } else if (matherr(&exc) == 0) {
                *__errno() = 33; /* EDOM */
            }
        } else {
            /* overflow */
            exc.type = 3; /* OVERFLOW */
            y = (float)(exc.arg2 * g_powfWrapperConsts.half);
            if (g_libVersion == 0) {
                exc.retval = g_powfWrapperConsts.huge;
                if (x < 0.0f && rint((double)y) != (double)y) {
                    exc.retval = g_powfWrapperConsts.negHuge;
                }
            } else {
                exc.retval = g_infinity;
                if (x < 0.0f && rint((double)y) != (double)y) {
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
        return (float)exc.retval;
    }

    if (z == 0.0f && finitef(x) != 0 && finitef(y) != 0) {
        /* underflow */
        exc.type = 4; /* UNDERFLOW */
        exc.name = "powf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        exc.retval = g_powfWrapperConsts.zero;
        if (g_libVersion == 2) {
            *__errno() = 34; /* ERANGE */
        } else if (matherr(&exc) == 0) {
            *__errno() = 34; /* ERANGE */
        }
        if (exc.err != 0) {
            *__errno() = exc.err;
        }
        return (float)exc.retval;
    }
    return z;
}

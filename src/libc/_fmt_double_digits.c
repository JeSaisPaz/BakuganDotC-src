// bdc 0x089b5450 _fmt_double_digits
#include "bdc.h"

/* The floating-point conversion core of `_vfprintf_r` (`%e %E %f %g %G`): converts `value` to
   text in the static buffer `g_fmtDoubleBuf` and returns it. `fmt` is `'f'`, `'e'` or `'E'` (the
   caller already resolved `%g` into one of them); `precision` is the digit count after the point
   (after the point for `f`, mantissa digits for `e`). Writes `'-'` into `*sign` (or as the first
   buffer byte when `sign` is NULL) for negative values; infinities and NaNs become `"Inf"`,
   `"-Inf"`, `"NaN"`. `strip_zeros != 0` (the `%g` case without `#`) removes trailing zeros and a
   dangling `'.'`. `e`-style output appends the exponent character, a sign, at least two exponent
   digits (`_itoa`).

   The binary does all double arithmetic through the libgcc soft-float helpers (`__cmpdf2`,
   `__muldf3`, `__subdf3`, `__divdf3`, `__fixdfsi`, `__floatsidf`, `__negdf2`); they are written
   here as the C operators they implement. No operand is ever NaN past the first check. */

#define FMT_INT_DIGITS_MAX 0xa3 /* integer digits produced at most */

char *_fmt_double_digits(double value, s32 precision, char fmt, s32 strip_zeros, char *sign)
{
    union {
        double d;
        u64 bits;
    } conv;
    u8 *buf;
    u32 hi;
    int n;        /* digits written to buf */
    int last;     /* index of the last character kept */
    int exp;      /* decimal exponent (integer digit count for 'f') */
    int digit;
    double frac;
    double rem;
    double eps;

    conv.d = value;
    hi = (u32)(conv.bits >> 32);
    if (((hi >> 20) & 0x7ff) == 0x7ff) {
        if ((hi & 0xfffff) == 0 && (u32)conv.bits == 0) {
            if ((hi & 0x80000000u) == 0) {
                strcpy((char *)g_fmtDoubleBuf, "Inf");
            } else {
                strcpy((char *)g_fmtDoubleBuf, "-Inf");
            }
        } else {
            strcpy((char *)g_fmtDoubleBuf, "NaN");
        }
        return (char *)g_fmtDoubleBuf;
    }

    n = 0;
    buf = g_fmtDoubleBuf;
    if (value < 0.0) {
        if (sign == NULL) {
            buf[0] = '-';
            buf++;
        } else {
            *sign = '-';
        }
        value = -value;
    }

    /* Integer part: digits from least significant, then reversed. */
    if (value >= 1.0) {
        double ip = floor(value);
        int i;
        int j;

        value = value - ip;
        while (n < FMT_INT_DIGITS_MAX) {
            digit = (int)__ieee754_fmod(ip, 10.0);
            buf[n++] = (char)(digit + '0');
            ip = (ip - (double)digit) / 10.0;
            if (ip < 1.0) {
                break;
            }
        }
        for (i = 0, j = n - 1; i < j; i++, j--) {
            u8 tmp = buf[i];

            buf[i] = buf[j];
            buf[j] = tmp;
        }
    }

    exp = n;
    last = precision;
    if (n == 0) {
        if (fmt == 'f') {
            buf[n++] = '0';
        } else {
            if (value != 0.0) {
                int e = 0;

                value = value * 10.0;
                while (value < 1.0 && e >= -0x3fc) {
                    e--;
                    value = value * 10.0;
                }
                exp = e - 1;
            }
            if (value >= 1.0) {
                value = value / 10.0;
                last = precision + 1;
            }
        }
    }

    frac = value * 10.0;
    if (exp >= 0) {
        if (fmt == 'f') {
            last += exp > 0 ? exp : 1;
        } else {
            last += 1;
        }
    }

    /* Fraction digits until `last` or until the remainder is within the accumulated error. */
    eps = 5.551115123125783e-17 /* 2^-54 */ * 10.0;
    for (;;) {
        digit = (int)frac;
        rem = frac - (double)digit;
        if (n >= last || eps > rem || rem > 1.0 - eps) {
            break;
        }
        buf[n++] = (char)(digit + '0');
        frac = rem * 10.0;
        eps = eps * 10.0;
    }
    if (rem >= 0.5) {
        digit++;
    }
    buf[n++] = (char)(digit + '0');
    while (n <= last) {
        buf[n++] = '0';
    }

    if (_fmt_round_digits((char *)buf, last + 1) == 0) {
        buf[0] = '1';
        last++;
        exp++;
    }

    if (precision == 0) {
        last--;
    } else {
        int point = 1;
        int i;

        if (fmt == 'f' && exp > 0) {
            point = exp;
        }
        for (i = last; point < i; i--) {
            buf[i] = buf[i - 1];
        }
        buf[point] = '.';
    }

    if (strip_zeros != 0) {
        char c = buf[last];

        while (last != 0 && c == '0') {
            last--;
            c = buf[last];
        }
        if (c == '.') {
            last--;
        }
    }

    if (fmt == 'f') {
        buf[last + 1] = '\0';
    } else {
        int i = last + 1;

        buf[i++] = fmt;
        if (exp >= 0) {
            buf[i++] = '+';
            if (exp != 0) {
                exp--;
            }
            if (exp < 10) {
                buf[i++] = '0';
            }
        } else {
            buf[i++] = '-';
            if (exp >= -9) {
                buf[i++] = '0';
            }
            exp = -exp;
        }
        _itoa(exp, (char *)&buf[i], 10);
    }
    return (char *)g_fmtDoubleBuf;
}

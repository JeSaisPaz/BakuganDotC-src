// bdc 0x089b5dfc _vfprintf_r
#include "bdc.h"

/* The full `printf` engine (newlib's `_vfprintf_r`, here with floating point): interprets `fmt`
   against the `va_list` `ap` (on the PSP a plain pointer to the spilled argument words; doubles and
   `long long` are 8-aligned) and writes to the stream `fp`, returning the number of characters
   produced. Supports flags `- + space # 0 '` (`'` = group thousands with commas via
   `_fmt_group_thousands`), width and precision (also `*`), length modifiers `h l ll L q`, and
   conversions `c d i u o x X p s n e E f g G`. Unknown conversion characters are printed literally;
   an incomplete `%` at the end ends the call.

   newlib 1.16 `vfprintf.c` structure with the Sony output path: a string sink
   (`fp->_flags & __SSTR`) is filled with memcpy at `fp->_p`, bounded by `fp->_w`; any other stream
   goes through `_vfprintf_write` on `fp->_file` (0 together with zero flags becomes 1, stdout)
   and is flushed at the end. `data` is unused: the format is scanned with `g_impurePtr`. The
   float conversions are formatted by `_fmt_double_digits` instead of newlib's `cvt`/`_dtoa`.
   Soft-float and 64-bit libgcc calls (`__cmpdf2`, `__negdf2`, `__floatsidf`,
   `__udivdi3`, `__umoddi3`) are written as C operators. */

#define SSTR      0x200 /* FILE _flags: string sink (sprintf/vsprintf) */

#define ALT       0x001 /* '#' alternate form */
#define HEXPREFIX 0x002 /* add 0x or 0X prefix */
#define LADJUST   0x004 /* '-' left adjustment */
#define LONGDBL   0x008 /* 'L' long double (read as double) */
#define LONGINT   0x010 /* 'l' long integer */
#define QUADINT   0x020 /* 'll'/'q' quad integer */
#define SHORTINT  0x040 /* 'h' short integer */
#define ZEROPAD   0x080 /* '0' zero (as opposed to blank) pad */
#define GROUPING  0x200 /* '\'' group thousands with ',' */

#define OCT 0
#define DEC 1
#define HEX 2

#define BUF     348 /* MAXEXP 308 + MAXFRACT 39 + 1 */
#define PADSIZE 16

#define to_digit(c) ((c) - '0')
#define is_digit(c) ((unsigned)to_digit(c) <= 9)
#define to_char(n)  ((char)((n) + '0'))

/* Writes len bytes at ptr to the string sink or the stream. */
#define PRINT(ptr, len)                                                         \
    do {                                                                        \
        if (fp->_flags & SSTR) {                                                \
            if ((len) < fp->_w) {                                               \
                memcpy(fp->_p, (ptr), (len));                                   \
                fp->_p += (len);                                                \
                fp->_w -= (len);                                                \
            } else {                                                            \
                memcpy(fp->_p, (ptr), fp->_w);                                  \
                fp->_p += fp->_w;                                               \
                fp->_w = 0;                                                     \
            }                                                                   \
        } else {                                                                \
            if (fp->_file == 0 && fp->_flags == 0) {                            \
                fp->_file = 1;                                                  \
            }                                                                   \
            _vfprintf_write(fp->_file, (void *)(ptr), (len), 0);               \
        }                                                                       \
    } while (0)

/* Writes howmany (if positive) bytes of padding from the 16-byte run with. */
#define PAD(howmany, with)                                                      \
    do {                                                                        \
        if ((n = (howmany)) > 0) {                                              \
            while (n > PADSIZE) {                                               \
                PRINT((with), PADSIZE);                                         \
                n -= PADSIZE;                                                   \
            }                                                                   \
            PRINT((with), n);                                                   \
        }                                                                       \
    } while (0)

/* `ap` is the caller's `va_list` as it is passed to a function (decayed): on the PSP the pointer to
   the spilled argument words itself, on an x86-64 port a pointer to the `va_list` state. VaArgs is
   that decayed type on any target, so `__builtin_va_arg` reads the caller's arguments either way.
   On the PSP 8-byte arguments are 8-aligned, which the binary does as `ap + (ap & 7)`. */
typedef __typeof__(&(*(__builtin_va_list *)0)[0]) VaArgs;

/* Fetches a signed / unsigned integer argument per the length flags. */
#define SARG()                                                                  \
    ((flags & QUADINT) ? (u64)__builtin_va_arg(args, s64)                       \
     : (flags & LONGINT) ? (u64)(s64)__builtin_va_arg(args, s32)                \
     : (flags & SHORTINT) ? (u64)(s64)(s16)__builtin_va_arg(args, s32)          \
     : (u64)(s64)__builtin_va_arg(args, s32))
#define UARG()                                                                  \
    ((flags & QUADINT) ? __builtin_va_arg(args, u64)                            \
     : (flags & LONGINT) ? (u64)__builtin_va_arg(args, u32)                     \
     : (flags & SHORTINT) ? (u64)(u16)__builtin_va_arg(args, s32)               \
     : (u64)__builtin_va_arg(args, u32))

int _vfprintf_r(_reent *data, FILE *fp, char *fmt, void *ap)
{
    VaArgs args = ap;
    const char *cp;       /* start of the literal run / the text to print */
    int ch;               /* character from fmt */
    s32 n;                /* scan result, then pad count */
    s32 m;                /* literal run length */
    s32 flags;            /* ALT .. GROUPING */
    s32 ret;              /* characters produced */
    s32 width;            /* width from format, or 0 */
    s32 prec;             /* precision from format, or -1 */
    char sign;            /* sign prefix (' ', '+', '-', or '\0') */
    u64 uquad;            /* integer argument */
    s32 base;             /* OCT, DEC or HEX */
    s32 dprec;            /* a copy of prec if [diouxX], 0 otherwise */
    s32 realsz;           /* field size expanded by dprec, sign, etc */
    s32 size;             /* size of the converted field or string */
    const char *xdigs;    /* digits for [xX] conversion */
    char buf[BUF];        /* space for %c, %[diouxX] */
    char ox[2];           /* space for 0x hex-prefix */
    u16 wc;
    s32 state;
    char *bufEnd;
    char *digit;          /* conversion of uquad (built backwards from bufEnd) or of the double */
    double value;         /* %[eEfgG] argument */
    double mag;           /* |value| for %g */
    double expo;          /* decimal exponent estimate for %g */
    s32 strip;            /* %g without '#': strip trailing zeros */
    const char *nul;

    (void)data;
    (void)localeconv(); /* result unused */
    state = 0;
    ret = 0;
    bufEnd = buf + BUF;

    for (;;) {
        cp = fmt;
        while ((n = _mbtowc_r(g_impurePtr, &wc, (u8 *)fmt, g_mbCurMax, &state)) > 0) {
            fmt += n;
            if (wc == '%') {
                fmt--;
                break;
            }
        }
        if ((m = fmt - cp) != 0) {
            PRINT(cp, m);
            ret += m;
        }
        if (n <= 0) {
            goto done;
        }
        if (fp->_w <= 0 && (fp->_flags & SSTR)) {
            goto done; /* string sink full */
        }
        fmt++; /* skip over '%' */

        flags = 0;
        dprec = 0;
        width = 0;
        prec = -1;
        sign = '\0';

    rflag:
        ch = *fmt++;
    reswitch:
        switch (ch) {
        case ' ':
            /* "If the space and + flags both appear, the space flag will be ignored." */
            if (!sign) {
                sign = ' ';
            }
            goto rflag;
        case '#':
            flags |= ALT;
            goto rflag;
        case '\'':
            flags |= GROUPING;
            goto rflag;
        case '*':
            width = __builtin_va_arg(args, s32);
            if (width >= 0) {
                goto rflag;
            }
            width = -width;
            /* FALLTHROUGH */
        case '-':
            flags |= LADJUST;
            goto rflag;
        case '+':
            sign = '+';
            goto rflag;
        case '.':
            if ((ch = *fmt++) == '*') {
                n = __builtin_va_arg(args, s32);
                prec = n < 0 ? -1 : n;
                goto rflag;
            }
            n = 0;
            while (is_digit(ch)) {
                n = 10 * n + to_digit(ch);
                ch = *fmt++;
            }
            prec = n < 0 ? -1 : n;
            goto reswitch;
        case '0':
            flags |= ZEROPAD;
            goto rflag;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            n = 0;
            do {
                n = 10 * n + to_digit(ch);
                ch = *fmt++;
            } while (is_digit(ch));
            width = n;
            goto reswitch;
        case 'L':
            flags |= LONGDBL;
            goto rflag;
        case 'h':
            flags |= SHORTINT;
            goto rflag;
        case 'l':
            if (*fmt == 'l') {
                fmt++;
                flags |= QUADINT;
            } else {
                flags |= LONGINT;
            }
            goto rflag;
        case 'q':
            flags |= QUADINT;
            goto rflag;
        case 'c':
            cp = buf;
            buf[0] = (char)__builtin_va_arg(args, s32);
            size = 1;
            sign = '\0';
            break;
        case 'D':
            flags |= LONGINT;
            /* FALLTHROUGH */
        case 'd':
        case 'i':
            uquad = SARG();
            if ((s64)uquad < 0) {
                uquad = -uquad;
                sign = '-';
            }
            base = DEC;
            goto number;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
            strip = 0;
            if (prec == -1) {
                prec = 6;
            }
            value = __builtin_va_arg(args, double);
            if (ch == 'g' || ch == 'G') {
                /* %g: exponent < -4 or >= precision selects e-style, else f-style */
                if (value != 0.0) {
                    mag = value;
                    if (mag < 0.0) {
                        mag = -mag;
                    }
                    expo = __ieee754_log10(mag);
                } else {
                    expo = 1.0;
                }
                if (expo < -4.0 || !(expo < (double)prec)) {
                    ch = (ch == 'g') ? 'e' : 'E';
                } else {
                    ch = 'f';
                }
                strip = 1;
            }
            if (flags & ALT) {
                if (prec == 0) {
                    prec = 1;
                }
                strip = 0;
            }
            /* writes '-' into sign for a negative value */
            digit = _fmt_double_digits(value, prec, (char)ch, strip, &sign);
            size = strlen(digit);
            if (flags & GROUPING) {
                digit = _fmt_group_thousands(digit, digit, digit + size, (u32 *)&size);
            }
            cp = digit;
            break;
        case 'n':
            if (flags & QUADINT) {
                *__builtin_va_arg(args, s64 *) = ret;
            } else if (flags & LONGINT) {
                *__builtin_va_arg(args, s32 *) = ret;
            } else if (flags & SHORTINT) {
                *__builtin_va_arg(args, s16 *) = (s16)ret;
            } else {
                *__builtin_va_arg(args, s32 *) = ret;
            }
            continue; /* no output */
        case 'O':
            flags |= LONGINT;
            /* FALLTHROUGH */
        case 'o':
            uquad = UARG();
            base = OCT;
            goto nosign;
        case 'p':
            /* "The argument shall be a pointer to void. The value of the pointer is converted to
               a sequence of printable characters, in an implementation-defined manner." */
            uquad = (u64)(uintptr_t)__builtin_va_arg(args, void *);
            base = HEX;
            xdigs = "0123456789abcdef";
            flags |= HEXPREFIX;
            ch = 'x';
            goto nosign;
        case 's':
            if ((cp = __builtin_va_arg(args, char *)) == NULL) {
                cp = "(null)";
            }
            if (prec >= 0) {
                /* can't use strlen; can only look for the NUL in the first `prec' characters */
                nul = memchr(cp, 0, prec);
                if (nul != NULL) {
                    size = nul - cp;
                    if (size > prec) {
                        size = prec;
                    }
                } else {
                    size = prec;
                }
            } else {
                size = strlen(cp);
            }
            sign = '\0';
            break;
        case 'U':
            flags |= LONGINT;
            /* FALLTHROUGH */
        case 'u':
            uquad = UARG();
            base = DEC;
            goto nosign;
        case 'X':
            xdigs = "0123456789ABCDEF";
            goto hex;
        case 'x':
            xdigs = "0123456789abcdef";
        hex:
            uquad = UARG();
            base = HEX;
            /* leading 0x/X only if non-zero */
            if ((flags & ALT) && uquad != 0) {
                flags |= HEXPREFIX;
            }
        nosign:
            /* unsigned conversions */
            sign = '\0';
        number:
            /* "... diouXx conversions ... if a precision is specified, the 0 flag will be
               ignored." */
            if ((dprec = prec) >= 0) {
                flags &= ~ZEROPAD;
            }
            /* "The result of converting a zero value with an explicit precision of zero is no
               characters." */
            digit = bufEnd;
            if (uquad != 0 || prec != 0) {
                switch (base) {
                case OCT:
                    do {
                        *--digit = to_char(uquad & 7);
                        uquad >>= 3;
                    } while (uquad != 0);
                    /* handle octal leading 0 */
                    if ((flags & ALT) && *digit != '0') {
                        *--digit = '0';
                    }
                    break;
                case DEC:
                    /* many numbers are 1 digit */
                    while (uquad >= 10) {
                        *--digit = to_char(uquad % 10);
                        uquad /= 10;
                    }
                    *--digit = to_char(uquad);
                    break;
                case HEX:
                    do {
                        *--digit = xdigs[uquad & 15];
                        uquad >>= 4;
                    } while (uquad != 0);
                    break;
                default:
                    cp = "bug in vfprintf: bad base";
                    size = strlen(cp);
                    goto skipsize;
                }
            }
            if (base == DEC && (flags & GROUPING)) {
                cp = _fmt_group_thousands(buf, digit, bufEnd, (u32 *)&size);
            } else {
                cp = digit;
                size = bufEnd - digit;
            }
        skipsize:
            break;
        default: /* "%?" prints ?, unless ? is NUL */
            if (ch == '\0') {
                goto done;
            }
            /* pretend it was %c with argument ch */
            cp = buf;
            buf[0] = (char)ch;
            size = 1;
            sign = '\0';
            break;
        }

        /* All reasonable formats wind up here. At this point, `cp' points to a string which (if
           not flags&LADJUST) should be padded out to `width' places. If flags&ZEROPAD, it should
           first be prefixed by any sign or other prefix; otherwise, it should be blank padded
           before the prefix is emitted. After any left-hand padding and prefixing, emit zeroes
           required by a decimal [diouxX] precision, then print the string proper, then emit
           zeroes required by any leftover floating precision; finally, if LADJUST, pad with
           blanks. */
        realsz = dprec > size ? dprec : size;
        if (sign) {
            realsz++;
        } else if (flags & HEXPREFIX) {
            realsz += 2;
        }

        /* right-adjusting blank padding */
        if ((flags & (LADJUST | ZEROPAD)) == 0) {
            PAD(width - realsz, g_vfprintfBlanks);
        }

        /* prefix */
        if (sign) {
            PRINT(&sign, 1);
        } else if (flags & HEXPREFIX) {
            ox[0] = '0';
            ox[1] = (char)ch;
            PRINT(ox, 2);
        }

        /* right-adjusting zero padding */
        if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD) {
            PAD(width - realsz, g_vfprintfZeroes);
        }

        /* leading zeroes from decimal precision */
        PAD(dprec - size, g_vfprintfZeroes);

        /* the string or number proper */
        PRINT(cp, size);

        /* left-adjusting padding (always blank) */
        if (flags & LADJUST) {
            PAD(width - realsz, g_vfprintfBlanks);
        }

        /* finally, adjust ret */
        ret += width > realsz ? width : realsz;
    }
done:
    if (!(fp->_flags & SSTR)) {
        _vfprintf_write(fp->_file, NULL, 0, 1);
    }
    return ret;
}

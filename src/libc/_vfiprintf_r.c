// bdc 0x089b779c _vfiprintf_r
#include "bdc.h"

/* Integer-only variant of the `printf` engine: newlib's `_vfiprintf_r`, the same code as
   `_vfprintf_r` minus the `e E f g G` conversions (they fall into the "print the character
   literally" default) and without the `localeconv`/`log10`/`_fmt_double_digits` dependencies.
   Handles `c d i u o x X p s n` with the usual flags (`- + space # 0 '`), width, precision and `h l
   ll q`, and returns the character count. Chosen by `vfprintf` when the format string contains
   no floating-point conversion.

   newlib 1.16 `vfprintf.c` (`INTEGER_ONLY`) structure with the Sony output path: a string sink
   (`fp->_flags & __SSTR`) is filled with memcpy at `fp->_p`, bounded by `fp->_w`; any other stream
   goes through `_vfiprintf_write` on `fp->_file` (0 together with zero flags becomes 1, stdout)
   and is flushed at the end. `data` is unused: the format is scanned with `g_impurePtr`.
   64-bit arithmetic is the libgcc `__udivdi3`/`__umoddi3` calls, written as C operators. */

#define SSTR      0x200 /* FILE _flags: string sink (sprintf/vsprintf) */

#define ALT       0x001 /* '#' alternate form */
#define HEXPREFIX 0x002 /* add 0x or 0X prefix */
#define LADJUST   0x004 /* '-' left adjustment */
#define LONGINT   0x010 /* 'l' long integer */
#define QUADINT   0x020 /* 'll'/'q' quad integer */
#define SHORTINT  0x040 /* 'h' short integer */
#define ZEROPAD   0x080 /* '0' zero (as opposed to blank) pad */
#define GROUPING  0x200 /* '\'' group thousands with ',' */

#define OCT 0
#define DEC 1
#define HEX 2

#define BUF     40
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
            _vfiprintf_write(fp->_file, (void *)(ptr), (len), 0);               \
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

/* `ap` is the PSP's `va_list`: a plain pointer to the spilled argument words, walked by hand
   (`args`). A 4-byte argument is the next word; an 8-byte argument is first 8-aligned, which the
   binary does as `args + (args & 7)` in bytes, then read as one little-endian 64-bit value. */
#define VA_ARG(type)   (args += 1, *(type *)(args - 1))
#define VA_ARG64(type)                                                          \
    (args = (u32 *)((uintptr_t)args + ((uintptr_t)args & 7)), args += 2,        \
     *(type *)(args - 2))

/* Fetches a signed / unsigned integer argument per the length flags. */
#define SARG()                                                                  \
    ((flags & QUADINT) ? (u64)VA_ARG64(s64)                                     \
     : (flags & LONGINT) ? (u64)(s64)VA_ARG(s32)                                \
     : (flags & SHORTINT) ? (u64)(s64)(s16)VA_ARG(s32)                          \
     : (u64)(s64)VA_ARG(s32))
#define UARG()                                                                  \
    ((flags & QUADINT) ? VA_ARG64(u64)                                          \
     : (flags & LONGINT) ? (u64)VA_ARG(u32)                                     \
     : (flags & SHORTINT) ? (u64)(u16)VA_ARG(s32)                               \
     : (u64)VA_ARG(u32))

int _vfiprintf_r(_reent *data, FILE *fp, char *fmt, void *ap)
{
    u32 *args = ap; /* spilled argument words */
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
    char *digit;          /* conversion of uquad, built backwards from bufEnd */
    const char *nul;

    (void)data;
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
            width = VA_ARG(s32);
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
                n = VA_ARG(s32);
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
            buf[0] = (char)VA_ARG(s32);
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
        case 'n':
            if (flags & QUADINT) {
                *VA_ARG(s64 *) = ret;
            } else if (flags & LONGINT) {
                *VA_ARG(s32 *) = ret;
            } else if (flags & SHORTINT) {
                *VA_ARG(s16 *) = (s16)ret;
            } else {
                *VA_ARG(s32 *) = ret;
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
            uquad = (u64)(uintptr_t)VA_ARG(void *);
            base = HEX;
            xdigs = "0123456789abcdef";
            flags |= HEXPREFIX;
            ch = 'x';
            goto nosign;
        case 's':
            if ((cp = VA_ARG(char *)) == NULL) {
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
            PAD(width - realsz, g_vfiprintfBlanks);
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
            PAD(width - realsz, g_vfiprintfZeroes);
        }

        /* leading zeroes from decimal precision */
        PAD(dprec - size, g_vfiprintfZeroes);

        /* the string or number proper */
        PRINT(cp, size);

        /* left-adjusting padding (always blank) */
        if (flags & LADJUST) {
            PAD(width - realsz, g_vfiprintfBlanks);
        }

        /* finally, adjust ret */
        ret += width > realsz ? width : realsz;
    }
done:
    if (!(fp->_flags & SSTR)) {
        _vfiprintf_write(fp->_file, NULL, 0, 1);
    }
    return ret;
}

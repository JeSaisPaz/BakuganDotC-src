// bdc 0x089b5d0c vfprintf
#include "bdc.h"

/* Newlib `vfprintf(fp, fmt, ap)`: scans `fmt` for a floating-point conversion (`e E f g G L`,
   after the characters below 'A' — flags, width, precision — of a `%` spec) and formats through
   the full formatter `_vfprintf_r` if one is found, otherwise through the integer-only
   `_vfiprintf_r`; both receive the stream's reentrancy struct `fp->_data`, the stream, the
   format and the `va_list`. */
int vfprintf(void *fp, const char *fmt, void *ap)
{
    FILE *stream = (FILE *)fp;
    const char *p = fmt;

    while (*p != '\0') {
        if (*p == '%' && p[1] != '\0') {
            p++;
            while (*p < 'A' && p[1] != '\0') {
                p++;
            }
            switch (*p) {
            case 'E':
            case 'G':
            case 'L':
            case 'e':
            case 'f':
            case 'g':
                return _vfprintf_r(stream->_data, stream, (char *)fmt, ap);
            default:
                break;
            }
        }
        p++;
    }
    return _vfiprintf_r(stream->_data, stream, (char *)fmt, ap);
}

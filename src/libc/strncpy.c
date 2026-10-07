// bdc 0x089b5104 strncpy
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `strncpy` at US `0x089b5144`. */

/* Copies at most `n` chars of `src` into `dst`, padding the rest of the `n`
   bytes with NULs once the terminator was copied. Returns `dst`. */
char *strncpy(char *dst, const char *src, size_t n)
{
    char *d = dst;

    while (n != 0) {
        n--;
        if ((*d++ = *src++) == '\0') {
            break;
        }
    }
    while (n != 0) {
        n--;
        *d++ = '\0';
    }
    return dst;
}

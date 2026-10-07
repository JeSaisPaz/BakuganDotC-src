// bdc 0x089b5068 strcmp
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `strcmp` at US `0x089b50a8`. */

/* Compares two NUL-terminated strings; returns the difference of the first
   mismatching bytes (as unsigned chars), 0 when equal. */
int strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (int)(u8)*s1 - (int)(u8)*s2;
}

// bdc 0x089b4f1c strcasecmp
#include "bdc.h"

/* Standard `strcasecmp` (newlib): compares `s1` and `s2` ignoring case, folding upper-case letters
   (class bit `_U` in `g_ctype`) to lower case. The scan loop classifies the characters as signed
   chars; the final difference is taken on the unsigned bytes, folded the same way. */

#define CTYPE_UPPER 0x01

int strcasecmp(const char *s1, const char *s2)
{
    int c1;
    int c2;

    while ((c1 = *s1) != 0) {
        if (g_ctype[c1 + 1] & CTYPE_UPPER) {
            c1 += 0x20;
        }
        c2 = *s2;
        if (g_ctype[c2 + 1] & CTYPE_UPPER) {
            c2 += 0x20;
        }
        if (c1 != c2) {
            break;
        }
        s1++;
        s2++;
    }
    c1 = (u8)*s1;
    c2 = (u8)*s2;
    if (g_ctype[c1 + 1] & CTYPE_UPPER) {
        c1 += 0x20;
    }
    if (g_ctype[c2 + 1] & CTYPE_UPPER) {
        c2 += 0x20;
    }
    return c1 - c2;
}

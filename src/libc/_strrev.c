// bdc 0x089b5348 _strrev
#include "bdc.h"

/* Reverses the NUL-terminated string `s` in place (swapping `s[i]` and `s[len-1-i]`) and returns
   `s`. The classic non-standard `strrev` used by `_itoa`. */

char *_strrev(char *s)
{
    int len = 0;
    int lo;
    int hi;
    char tmp;

    while (s[len] != '\0') {
        len++;
    }
    for (lo = 0, hi = len - 1; lo < hi; lo++, hi--) {
        tmp = s[lo];
        s[lo] = s[hi];
        s[hi] = tmp;
    }
    return s;
}

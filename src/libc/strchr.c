// bdc 0x089b5030 strchr
#include "bdc.h"

/* Standard `strchr`: returns a pointer to the first byte of `s` equal to `(unsigned char)c`, or
   NULL when the terminator is reached first (searching for 0 returns the terminator). */

char *strchr(const char *s, int c)
{
    u8 want = (u8)c;
    u8 ch;

    while ((ch = (u8)*s) != 0 && ch != want) {
        s++;
    }
    if (ch != want) {
        return NULL;
    }
    return (char *)s;
}

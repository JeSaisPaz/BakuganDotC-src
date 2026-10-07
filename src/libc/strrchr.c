// bdc 0x089b5164 strrchr
#include "bdc.h"

/* Standard `strrchr`: returns a pointer to the last byte of `s` equal to `(char)c`, or NULL;
   searching for 0 returns the terminator. */

char *strrchr(const char *s, int c)
{
    char want = (char)c;
    const char *last = NULL;

    for (; *s != '\0'; s++) {
        if (*s == want) {
            last = s;
        }
    }
    if (want == '\0') {
        last = s;
    }
    return (char *)last;
}

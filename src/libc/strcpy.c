// bdc 0x089b509c strcpy
#include "bdc.h"

/* Standard newlib `strcpy`: copies NUL-terminated `src` (including terminator) into `dst`, returns
   `dst`. */

char *strcpy(char *dst, const char *src)
{
    char *d = dst;

    while ((*d++ = *src++) != '\0') {
    }
    return dst;
}

// bdc 0x089b4ee8 memset
#include "bdc.h"

/* Standard newlib `memset`: fills `n` bytes at `s` with `(unsigned char)c` and returns `s`. */

void *memset(void *s, int c, size_t n)
{
    u8 *dst = s;

    for (; n != 0; n--) {
        *dst++ = (u8)c;
    }
    return s;
}

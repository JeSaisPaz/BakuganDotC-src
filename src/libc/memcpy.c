// bdc 0x089b4ea8 memcpy
#include "bdc.h"

/* Standard newlib `memcpy`: copies `n` bytes from `src` to `dst`, returns `dst`. */

void *memcpy(void *dst, const void *src, size_t n)
{
    u8 *d = dst;
    const u8 *s = src;

    for (; n != 0; n--) {
        *d++ = *s++;
    }
    return dst;
}

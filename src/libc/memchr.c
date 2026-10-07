// bdc 0x089bb2dc memchr
#include "bdc.h"

/* Ported from US legacy by byte match (unique masked-opcode hash): `memchr` at US `0x089bb31c`. */

/* Returns a pointer to the first of the first `n` bytes of `s` equal to
   `(u8)c`, or NULL. */
void *memchr(const void *s, int c, size_t n)
{
    const u8 *p = (const u8 *)s;
    u8 ch = (u8)c;

    while (n != 0) {
        n--;
        if (*p == ch) {
            return (void *)p;
        }
        p++;
    }
    return NULL;
}

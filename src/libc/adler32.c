// bdc 0x08a02644 adler32
#include "bdc.h"

#define ADLER_BASE 65521u /* largest prime below 65536 */
#define ADLER_NMAX 5552   /* bytes before s2 could overflow 32 bits */

/* zlib adler32: updates the Adler-32 checksum `adler` over `len` bytes of
   `buf` and returns `s1 | s2 << 16`. */
u32 adler32(u32 adler, const u8 *buf, int len)
{
    u32 s1 = adler & 0xffff;
    u32 s2 = adler >> 16;

    while (len != 0) {
        int chunk = len < ADLER_NMAX ? len : ADLER_NMAX;

        len -= chunk;
        do {
            s1 += *buf++;
            s2 += s1;
        } while (--chunk != 0);
        s1 %= ADLER_BASE;
        s2 %= ADLER_BASE;
    }
    return s1 | (s2 << 16);
}

// bdc 0x08908714 BtlScbByteSwap32
#include "bdc.h"

/* Stores the byte-swapped (big <-> little endian) value of `*src` into `*dst`. */
void BtlScbByteSwap32(u32 *src, u32 *dst)
{
    u32 v = *src;

    *dst = (v & 0xff0000) >> 8 | v >> 24 | v << 24 | (v & 0xff00) << 8;
}

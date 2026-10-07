// bdc 0x08908754 BtlScbByteSwapBuffer
#include "bdc.h"

/* Byte-swaps `size / 4` 32-bit words from `src` to `dst` (`BtlScbByteSwap32`); used in place on
   loaded `.scb`/`.MAF` files. The compiled loop keeps its counter in v0 across the call, relying on
   `BtlScbByteSwap32` leaving v0 alone; this is a plain counted loop. */
void BtlScbByteSwapBuffer(u32 *src, u32 *dst, u32 size)
{
    u32 count = size >> 2;
    u32 i;

    for (i = 0; i < count; i++) {
        BtlScbByteSwap32(src, dst);
        src++;
        dst++;
    }
}

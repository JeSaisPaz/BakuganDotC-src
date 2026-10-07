// bdc 0x089be4c0 CoreLzssGetSize
#include "bdc.h"

/* Returns the uncompressed size stored little-endian in the first four bytes of an LZSS-packed
   blob (assembled byte-wise, so the blob needs no alignment), or 0 for a NULL blob. Used to size
   the destination buffer before `CoreLzssDecompress`; `GfxInitBootResources` sums it over its
   14 compressed TIM2 textures. */
u32 CoreLzssGetSize(const u8 *blob)
{
    if (blob == NULL)
        return 0;
    return (u32)blob[0] | (u32)blob[1] << 8 | (u32)blob[2] << 16 | (u32)blob[3] << 24;
}

// bdc 0x089be4f8 CoreLzssDecompress
#include "bdc.h"

/* Decompresses an LZSS blob (4-byte size header, see `CoreLzssGetSize`) into `dst` with
   `CoreLzssDecodeRaw` and returns the number of bytes produced (0 for a NULL `dst` or an empty
   blob). */
s32 CoreLzssDecompress(const u8 *blob, u8 *dst)
{
    s32 size = 0;

    if (dst != NULL) {
        size = (s32)CoreLzssGetSize(blob);
        if (size > 0) {
            CoreLzssDecodeRaw(blob + 4, dst, size);
        }
    }
    return size;
}

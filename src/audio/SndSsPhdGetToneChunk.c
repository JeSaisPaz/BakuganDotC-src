// bdc 0x08a215b0 SndSsPhdGetToneChunk
#include "bdc.h"

/* Locates the `PPTN` (tone list) chunk of a PHD sound-bank header (offset `phd+0x14`), checking
   the `PPHD` and chunk magics. Writes the chunk pointer to `*outChunk` once the offset is valid.
   Returns 0, or -1 on a bad argument, bad magic or missing chunk. */

static u32 ReadMagic(const u8 *p)
{
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

s32 SndSsPhdGetToneChunk(const void *phd, void **outChunk)
{
    const u8 *base = (const u8 *)phd;
    const u8 *chunk;
    s32 offset;
    s32 result = -1;

    if (phd != NULL && ReadMagic(base) == 0x50504844 && outChunk != NULL) {
        offset = ((const SndSsPhdHeader *)phd)->toneOffset;
        chunk = base + offset;
        result = -1;
        if (offset != -1) {
            *outChunk = (void *)chunk;
            result = 0;
            if (ReadMagic(chunk) != 0x5050544e) {
                result = -1;
            }
        }
    }
    return result;
}

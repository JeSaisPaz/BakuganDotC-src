// bdc 0x08a21404 SndSsPhdGetTone
#include "bdc.h"

/* Returns in `*outEntry` the 0x60-byte tone entry `index` of a PHD bank header's `PPTN` chunk
   (`SndSsPhdGetToneChunk`; valid indices `chunk+0x10..+0x14`, entries from `chunk+0x20`). Returns
   0, or -1 when out of range or the entry is unused (VAG index and noise field both -1). */

s32 SndSsPhdGetTone(const void *phd, u32 index, void **outEntry)
{
    const u32 *chunk;
    const u32 *entry;

    if (SndSsPhdGetToneChunk(phd, (void **)&chunk) < 0) {
        return -1;
    }
    if (index < chunk[4] || index > chunk[5]) {
        return -1;
    }
    entry = chunk + 8 + (index - chunk[4]) * 24;
    *outEntry = (void *)entry;
    if (entry[0] == 0xFFFFFFFFu && entry[3] == 0xFFFFFFFFu) {
        return -1;
    }
    return 0;
}

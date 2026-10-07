// bdc 0x08a21384 SndSsPhdGetVag
#include "bdc.h"

/* Returns in `*outEntry` the 0x10-byte VAG entry `index` of a PHD bank header's `PPVA` chunk
   (`SndSsPhdGetVagChunk`; valid indices `chunk+0x10..+0x14`, entries from `chunk+0x20`). Returns
   0, or -1 when out of range or the entry's offset is -1. */

typedef struct SndSsVagEntry {
  s32 offset;
  u8 pad04[0xc];
} SndSsVagEntry;

typedef struct SndSsVagChunk {
  u8 magic[4];
  u8 pad04[0xc];
  u32 firstIndex;
  u32 lastIndex;
  u8 pad18[8];
  SndSsVagEntry entries[1];
} SndSsVagChunk;

s32 SndSsPhdGetVag(const void *phd, u32 index, void **outEntry)
{
  SndSsVagChunk *chunk;
  s32 result;

  result = SndSsPhdGetVagChunk(phd, (void **)&chunk);
  if (result < 0) {
    return -1;
  }
  result = -1;
  if (index >= chunk->firstIndex && index <= chunk->lastIndex) {
    SndSsVagEntry *entry = &chunk->entries[index - chunk->firstIndex];
    *outEntry = entry;
    result = -1;
    if (entry->offset != -1) {
      result = 0;
    }
  }
  return result;
}

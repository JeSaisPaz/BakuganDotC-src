// bdc 0x08a21520 SndSsPhdGetVagChunk
#include "bdc.h"

/* Locates the `PPVA` (VAG list) chunk of a PHD sound-bank header: checks the `PPHD` magic, follows
   the offset at `phd+0x18` and checks the chunk magic. Returns 0, or -1 on a bad header/missing
   chunk. */

static u32 SndSsPhdMagic(const u8 *p) {
  return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

s32 SndSsPhdGetVagChunk(const void *phd, void **outChunk)
{
  const SndSsPhdHeader *hdr = (const SndSsPhdHeader *)phd;
  const u8 *chunk;
  s32 result;

  result = -1;
  if (hdr != NULL && SndSsPhdMagic(hdr->magic) == 0x50504844 && outChunk != NULL) {
    chunk = (const u8 *)hdr + hdr->vagOffset;
    result = -1;
    if (hdr->vagOffset != -1) {
      *outChunk = (void *)chunk;
      result = 0;
      if (SndSsPhdMagic(chunk) != 0x50505641) {
        result = -1;
      }
    }
  }
  return result;
}

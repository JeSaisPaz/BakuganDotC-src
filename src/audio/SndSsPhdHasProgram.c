// bdc 0x08a216d0 SndSsPhdHasProgram
#include "bdc.h"

/* Returns 0 when program `program` (< 0x80) is defined in the PHD header's `PPPG` chunk (offset
   entry != -1), else -1. */

typedef struct PppgChunk {
  u8 header[0x20];
  s32 offsets[0x80];
} PppgChunk;

s32 SndSsPhdHasProgram(const void *phd, u32 program)
{
  const PppgChunk *chunk;
  s32 result;
  s32 status;

  result = -1;
  chunk = NULL;
  if (program < 0x80) {
    status = SndSsPhdGetProgramChunk(phd, (void **)&chunk);
    result = -1;
    if (status >= 0 && chunk->offsets[program] != -1) {
      result = 0;
    }
  }
  return result;
}

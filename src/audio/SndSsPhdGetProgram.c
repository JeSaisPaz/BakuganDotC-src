// bdc 0x08a214a4 SndSsPhdGetProgram
#include "bdc.h"

/* Returns in `*outProgram` program `program` of a PHD bank header (`phd + PPPG.offsets[program]`)
   after checking it exists (`SndSsPhdHasProgram`). Returns 0 or -1. */

s32 SndSsPhdGetProgram(const void *phd, u32 program, void **outProgram)
{
    const u32 *chunk;

    if (SndSsPhdHasProgram(phd, program) < 0) {
        return -1;
    }
    if (SndSsPhdGetProgramChunk(phd, (void **)&chunk) < 0) {
        return -1;
    }
    *outProgram = (void *)((const u8 *)phd + chunk[8 + program]);
    return 0;
}

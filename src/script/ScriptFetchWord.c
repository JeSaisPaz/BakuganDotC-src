// bdc 0x089c9e1c ScriptFetchWord
#include "bdc.h"

/* Reads the next 32-bit little-endian bytecode word from the script's operand cursor
   (`operand`), advances the cursor by 4 and returns the word. Assembled from four byte loads,
   so it works on unaligned code. */

u32 ScriptFetchWord(Script *script)
{
    const u8 *p = script->operand;
    u32 lo = p[0] | ((u32)p[1] << 8);
    u32 hi = ((u32)p[2] << 16) | ((u32)p[3] << 24);

    script->operand = (u8 *)&p[4];
    return hi | lo;
}

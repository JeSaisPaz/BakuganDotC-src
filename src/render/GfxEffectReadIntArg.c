// bdc 0x0881d7d4 GfxEffectReadIntArg
#include "bdc.h"

/* Reads the next integer argument of an effect command and advances the argument cursor `*cursor`.
   Returns 0 once all `cmd[1]` (byte) arguments are consumed. The argument kind is 3 bits per
   argument in the command's u16 at `+0`: kind 3 is a random range, reading two words `lo, hi` and
   returning `lo + ((rand16 * (hi - lo)) >> 16)` (rand16 = top 16 bits of a random word, VFPU
   `vrndi`), or `lo` plus the whole random word when `hi == lo`; any other kind returns the word as
   is. */

int GfxEffectReadIntArg(GfxEffect *effect, s32 *args, s32 *cursor, u16 *cmd)
{
    s32 idx = *cursor;
    s32 lo;
    s32 hi;
    u32 rnd;
    u32 scaled;

    if (idx >= (s32)(u8)cmd[1]) {
        return 0;
    }
    lo = args[idx];
    if ((((s32)cmd[0] >> ((idx * 3) & 0x1f)) & 7) == 3) {
        *cursor = idx + 1;
        hi = args[idx + 1];
        *cursor = idx + 2;
        rnd = PlatformRandU32();
        if (hi - lo != 0) {
            scaled = ((rnd >> 16) * (u32)(hi - lo)) >> 16;
        } else {
            scaled = rnd;
        }
        return lo + (s32)scaled;
    }
    *cursor = idx + 1;
    return lo;
}

// bdc 0x089becd4 CoreRandNext
#include "bdc.h"

/* Game-wide pseudo-random number generator: steps the four-word lagged generator `g_randState`
   once and returns the new word, or — when `n != 0` — a value in `[0, n)`: the high word of the
   64-bit product `word * n` (computed with `__muldi3`). Used by gameplay code to pick random
   list entries and parameters (e.g. `ActorCrystalRandomStyle`).

   Step: `a = (s1 << 2) | (s0 >> 30)`, `b = (s3 << 1) | (s2 >> 31)`, state shifts down one word
   and `s0 = a ^ b`. */
u32 CoreRandNext(u32 n)
{
    u32 a;
    u32 b;
    u32 word;

    a = g_randState[1] * 2;
    if (g_randState[0] & 0x80000000) {
        a++;
    }
    a = a * 2;
    if (g_randState[0] & 0x40000000) {
        a++;
    }
    b = g_randState[3] * 2;
    if (g_randState[2] & 0x80000000) {
        b++;
    }
    g_randState[3] = g_randState[2];
    g_randState[2] = g_randState[1];
    word = a ^ b;
    g_randState[1] = g_randState[0];
    g_randState[0] = word;
    if (n != 0) {
        return (u32)(((u64)word * (u64)n) >> 32);
    }
    return word;
}

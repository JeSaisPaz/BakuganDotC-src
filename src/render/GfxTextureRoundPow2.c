// bdc 0x089f7164 GfxTextureRoundPow2
#include "bdc.h"

/* Rounds `n` up to a power of two (0 stays 0): counts its set bits with `MathPopCount32`, walks to
   just past the highest set bit and halves that back when `n` already was a power of two. */

u32 GfxTextureRoundPow2(void *tex, u32 n)
{
  int bits;
  u32 mask;
  int seen;

  bits = MathPopCount32(n);
  seen = 0;
  mask = 1;
  if (bits == 0) {
    return 0;
  }
  if (0 < bits) {
    do {
      if ((n & mask) != 0) {
        seen = seen + 1;
      }
      mask = mask << 1;
    } while (seen < bits);
  }
  if (bits < 2) {
    mask = (int)mask >> 1;
  }
  return mask;
}

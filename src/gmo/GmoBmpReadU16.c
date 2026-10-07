// bdc 0x08a28050 GmoBmpReadU16
#include "bdc.h"

/* Reads a 16-bit value from `p` into `*out` and returns `p + 1`. Handles odd (unaligned) `p` bytewise. */

const u16 *GmoBmpReadU16(const u16 *p, u16 *out)
{
  const u8 *b = (const u8 *)p;
  u32 hi;
  u32 lo;

  if (((uintptr_t)p & 1) == 0) {
    *out = *p;
  } else {
    b++;
    hi = *b;
    b--;
    lo = *b;
    *out = (u16)((hi << 8) | lo);
  }
  return p + 1;
}

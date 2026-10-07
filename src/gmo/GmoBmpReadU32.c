// bdc 0x08a28084 GmoBmpReadU32
#include "bdc.h"

/* Reads a little-endian 32-bit value from a possibly unaligned `p` into `*out` and returns `p + 1`.
    */

const u32 *GmoBmpReadU32(const u32 *p, u32 *out)
{
  const u8 *b = (const u8 *)p;
  u32 b0;
  u32 b1;
  u32 b2;
  u32 b3;

  if (((uintptr_t)p & 3) != 0) {
    b0 = *b++;
    b1 = *b++;
    b2 = *b++;
    b3 = *b;
    *out = b3 << 0x18 | b2 << 0x10 | b1 << 8 | b0;
    return p + 1;
  }
  *out = *p;
  return p + 1;
}

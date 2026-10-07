// bdc 0x08a277a4 GmoTgaReadU16
#include "bdc.h"

/* Reads a little-endian 16-bit value from `p` into `*out` and returns `p + 1` (one u16 on). An unaligned
   `p` is assembled from two byte loads; an aligned one uses a halfword load. */

const u16 *GmoTgaReadU16(const u16 *p, u16 *out)
{
  const u8 *lo = (const u8 *)p;
  const u8 *hi = lo;
  hi++;
  if (((uintptr_t)p & 1) != 0) {
    *out = (u16)((*hi << 8) | *lo);
    return p + 1;
  }
  *out = *p;
  return p + 1;
}

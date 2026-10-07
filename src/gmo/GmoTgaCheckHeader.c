// bdc 0x08a275a4 GmoTgaCheckHeader
#include "bdc.h"

/* Returns true when `data` is non-null, `size` > 17 and the TGA image type (`data[2]`, RLE bit 8 masked)
   is 1 (colour-mapped) or 2 (true-colour); otherwise 0. */

s32 GmoTgaCheckHeader(const u8 *data, s32 size)
{
  s32 ok = 0;
  if (data != (const u8 *)0 && size > 0x11) {
    ok = (u32)((data[2] & ~8) - 1) < 2;
  }
  return ok;
}

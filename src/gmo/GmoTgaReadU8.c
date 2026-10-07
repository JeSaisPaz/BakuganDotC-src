// bdc 0x08a27794 GmoTgaReadU8
#include "bdc.h"

/* Reads one byte from `p` into `*out` and returns `p + 1`. */

const u8 *GmoTgaReadU8(const u8 *p, u8 *out)

{
  *out = *p;
  return p + 1;
}


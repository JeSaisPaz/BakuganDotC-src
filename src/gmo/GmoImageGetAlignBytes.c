// bdc 0x08a25df0 GmoImageGetAlignBytes
#include "bdc.h"

/* Returns the byte size of one width-alignment unit of an image: `bpp (+0x17) * widthAlign (+0x18)
   / 8`, 0 for NULL. */

s32 GmoImageGetAlignBytes(const GmoImage *self)
{
  s32 bytes = 0;

  if (self != NULL) {
    bytes = (s32)((u32)self->bpp * (u32)self->widthAlign) >> 3;
  }
  return bytes;
}

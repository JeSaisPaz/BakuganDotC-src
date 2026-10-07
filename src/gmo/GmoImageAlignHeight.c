// bdc 0x08a25e84 GmoImageAlignHeight
#include "bdc.h"

/* Rounds `height` up to the image's height alignment (`+0x19`, a power of two). 0 for NULL. */

u32 GmoImageAlignHeight(const GmoImage *self, u32 height)
{
  u32 result = 0;
  u32 mask;

  if (self != NULL) {
    mask = self->heightAlign - 1;
    result = (height + mask) & ~mask;
  }
  return result;
}

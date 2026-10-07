// bdc 0x08a25eec GmoImageGetLevelWidth
#include "bdc.h"

/* Returns the width of mip level `level`: the base width (`+0x12`) halved (rounding up) `level`
   times when the mipmap mode (`+0x28`) is 1, else the base width. */

u32 GmoImageGetLevelWidth(const GmoImage *self, s32 level)

{
  u32 result;
  s32 i;

  if (self == (GmoImage *)0x0) {
    return 0;
  }
  result = self->width;
  if (level > 0 && self->mipmapMode == 1) {
    for (i = 0; i < level; i++) {
      result = (result + 1) / 2;
    }
  }
  return result;
}

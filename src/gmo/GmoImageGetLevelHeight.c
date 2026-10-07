// bdc 0x08a25f3c GmoImageGetLevelHeight
#include "bdc.h"

/* Returns the height of mip level `level`: the base height (`+0x14`) halved (rounding up) `level`
   times when the mipmap mode (`+0x28`) is 1, else the base height. */

u32 GmoImageGetLevelHeight(const GmoImage *self, s32 level)

{
  u32 result;
  s32 i;

  if (self == (GmoImage *)0x0) {
    return 0;
  }
  result = self->height;
  if (level > 0 && self->mipmapMode == 1) {
    for (i = 0; i < level; i++) {
      result = (result + 1) / 2;
    }
  }
  return result;
}

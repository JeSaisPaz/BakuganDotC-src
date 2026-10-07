// bdc 0x08a11e60 GmoImageLevelSize
#include "bdc.h"

/* Returns the byte size of mip level `level` of an image: width/height halved `level` times, the
   row padded to `alignW * 8` bits and the height to `alignH`. */

int GmoImageLevelSize(int bpp, int w, int h, int alignW, int alignH, int level)

{
  s32 i;
  s32 mask;
  s32 bits;

  if (-1 < level - 1) {
    i = 0;
    do {
      i = i + 1;
      w = (w + 1) / 2;
      h = (h + 1) / 2;
    } while (i != level);
  }
  mask = alignW * 8 - 1;
  bits = (mask + bpp * w) & ~mask;
  return (bits / 8) * ((h + alignH - 1) & ~(alignH - 1));
}

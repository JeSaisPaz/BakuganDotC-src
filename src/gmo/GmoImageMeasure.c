// bdc 0x08a11ee0 GmoImageMeasure
#include "bdc.h"

/* Measuring pass for an image against the two-pass arena `plan`: reserves the image header
   (`headerSize`, 16-aligned) and the `levels * frames` table of pixel pointers in pool 0, then,
   unless `headerOnly`, the pixel data of every level of every frame in pool `pool` with alignment
   `align` (`GmoImagePlanReserve`, sizes from `GmoImageLevelSize`). Always returns 1. */

int GmoImageMeasure(void *image, u32 fmt, int order, int w, int h, int alignW, int alignH,
                    int levels, int frames, int mipmapped, int unused10, int pool, u32 align,
                    int headerSize, int headerOnly, void *plan)

{
  int bpp;
  int frame;
  int level;

  GmoImagePlanReserve(plan, 0, 0x10, headerSize);
  GmoImagePlanReserve(plan, 0, (u32)__alignof__(void *), levels * frames * (int)sizeof(void *));
  if (headerOnly != 0) {
    return 1;
  }
  bpp = GmoImageFormatBits(fmt);
  alignW = (alignW + 0xf) & ~0xf;
  if (order == 1) {
    alignH = (alignH + 7) & ~7;
  }
  for (frame = 0; frame < frames; frame++) {
    for (level = 0; level < levels; level++) {
      GmoImagePlanReserve(plan, pool, align,
                          GmoImageLevelSize(bpp, w, h, alignW, alignH,
                                            mipmapped == 1 ? level : 0));
    }
  }
  return 1;
}

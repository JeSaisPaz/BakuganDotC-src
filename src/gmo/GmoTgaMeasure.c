// bdc 0x08a27d04 GmoTgaMeasure
#include "bdc.h"

/* Measure pass of `GmoTextureLoadTga`: validates the header and reserves the palette
   (colour-mapped images) and pixel images with `GmoImageMeasure`. Returns 1, or 0 on a bad file. */

s32 GmoTgaMeasure(void *img, void *data, u32 size, s32 index, void *arena)
{
  GmoTgaHeader hdr;
  u32 fmt;

  if (img == NULL || arena == NULL) {
    return 0;
  }
  if (GmoTgaCheckHeader(data, size) == 0) {
    return 0;
  }
  GmoTgaReadHeader(data, &hdr);
  if (hdr.colorMapType != 0) {
    /* palette image: cmapLength x 1, format 3 (8888) */
    GmoImagePlanReserveTracks(1, arena);
    GmoImageMeasure(NULL, 3, 0, hdr.cmapLength, 1, 0x10, 1, 1, 1, 2, 3, 1, 0x10, 0, 0, arena);
  }
  fmt = GmoTgaBppToFormat(hdr.pixelDepth);
  GmoImagePlanReservePalettes(1, arena);
  GmoImageMeasure(NULL, fmt, 0, hdr.width, hdr.height, 0x10, 1, 1, 1, 1, 3, 1, 0x80, 0, 0, arena);
  return 1;
}

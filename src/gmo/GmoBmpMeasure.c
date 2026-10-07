// bdc 0x08a281ac GmoBmpMeasure
#include "bdc.h"

/* Measure pass of `GmoTextureLoadBmp`: validates the file (`GmoBmpCheckHeader`,
   `GmoBmpReadHeader`) and reserves, in the two-pass arena `arena`, the palette image (16 or 256
   8888 entries, only when the file carries a colour table and the bit depth is below 16) and the
   pixel image (format from `GmoBmpBppToFormat`, width x |height|). Returns 1, or 0 when `img` or
   `arena` is null or the file is not a BMP. */

s32 GmoBmpMeasure(void *img, void *data, u32 size, s32 index, void *arena)
{
  GmoBmpHeader hdr;
  s32 paletteEntries;
  s32 colors;
  u32 fmt;
  s32 height;

  if (img == NULL || arena == NULL) {
    return 0;
  }
  if (GmoBmpCheckHeader((const u16 *)data, size) == 0) {
    return 0;
  }
  GmoBmpReadHeader(data, &hdr);
  /* colour table between the 0x36-byte headers and the pixel data, 4 bytes per entry */
  colors = ((s32)hdr.dataOffset - 0x36) / 4;
  if (colors > 0) {
    paletteEntries = 0x10;
    if (colors > 0x10) {
      paletteEntries = 0x100;
    }
    if ((s16)hdr.bitCount < 0x10) {
      GmoImagePlanReserveTracks(1, arena);
      GmoImageMeasure(NULL, 3, 0, paletteEntries, 1, 0x10, 1, 1, 1, 2, 3, 1, 0x10, 0, 0, arena);
    }
  }
  fmt = GmoBmpBppToFormat((s16)hdr.bitCount, hdr.compression);
  height = hdr.height;
  if (height < -height) {
    height = -height;
  }
  GmoImagePlanReservePalettes(1, arena);
  GmoImageMeasure(NULL, fmt, 0, hdr.width, height, 0x10, 1, 1, 1, 1, 3, 1, 0x80, 0, 0, arena);
  return 1;
}

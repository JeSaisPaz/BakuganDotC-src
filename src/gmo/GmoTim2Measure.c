// bdc 0x08a26d64 GmoTim2Measure
#include "bdc.h"

/* Measure pass of `GmoTextureLoadTim2`: validates `img`, `arena`, the file
   (`GmoTim2CheckHeader`) and picture `index` (`GmoTim2GetPicture`). When the picture has
   mipmap levels it reserves the pixel image (`width` x `height`, `mipMapTextures` levels, format
   `g_gmoTim2FormatMap`[`imageType`]); when `clutType & 7` is set it reserves the CLUT image
   (`clutColors` x 1, format `g_gmoTim2FormatMap`[`clutType & 7`]), both with `GmoImageMeasure`.
   Returns 1, or 0 when an argument, the file or the picture is invalid. */

s32 GmoTim2Measure(void *img, void *data, u32 size, s32 index, void *arena)
{
  GfxTim2Picture *pic;
  s8 levels;
  u32 clutType;
  s32 w;
  s32 h;
  u32 fmt;

  if (img == NULL || arena == NULL) {
    return 0;
  }
  if (GmoTim2CheckHeader(data, size) == 0) {
    return 0;
  }
  pic = (GfxTim2Picture *)GmoTim2GetPicture(data, size, index);
  if (pic == NULL) {
    return 0;
  }
  levels = pic->mipMapTextures;
  if (levels != 0) {
    w = (s16)pic->width;
    h = (s16)pic->height;
    fmt = g_gmoTim2FormatMap[(s8)pic->imageType];
    GmoImagePlanReservePalettes(1, arena);
    GmoImageMeasure(NULL, fmt, 0, w, h, 0x10, 1, levels, 1, 1, 3, 1, 0x80, 0, 0, arena);
  }
  clutType = pic->clutType & 7;
  if (clutType != 0) {
    fmt = g_gmoTim2FormatMap[clutType];
    w = (s16)pic->clutColors;
    GmoImagePlanReserveTracks(1, arena);
    GmoImageMeasure(NULL, fmt, 0, w, 1, 0x10, 1, 1, 1, 2, 3, 1, 0x10, 0, 0, arena);
  }
  return 1;
}

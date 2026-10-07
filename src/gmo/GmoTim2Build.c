// bdc 0x08a27168 GmoTim2Build
#include "bdc.h"

/* Build pass of `GmoTextureLoadTim2`: returns 0 when `img` or `arena` is NULL, the data is not a
   TIM2 file or picture `index` is missing. Otherwise empties the texture (`GmoTextureReset`); when
   the picture has mip levels it carves the pixel image from `arena`, builds it
   (`GmoImageBuild`, format `g_gmoTim2FormatMap`[`imageType`]) and converts every level
   (`GmoTim2ConvertPixels`, halving the size per level when there is a mipmap header). When
   `clutType & 7` is set it builds the CLUT image (`clutColors` x 1) and converts the palette found
   after the pixel data; a 256-colour CSM1 CLUT (bit 7 of `clutType` clear) of format 1 or 3 is then
   unswizzled by swapping the middle 8-entry runs of every 32-entry block. Flushes the data cache
   and returns 1. */

s32 GmoTim2Build(void *img, void *data, u32 size, s32 index, void *arena)
{
  GmoTexture *tex = (GmoTexture *)img;
  GfxTim2Picture *pic;
  GmoImage *image;
  u32 fmt;
  u32 clutType;
  s32 levels;
  s32 width;
  s32 height;
  s32 level;
  s32 hasMips;
  s32 i;
  s32 j;
  u8 *src;
  u32 *clut;
  u32 lo;
  u32 hi;

  if (img == NULL || arena == NULL || GmoTim2CheckHeader((const u32 *)data, size) == 0) {
    return 0;
  }
  pic = (GfxTim2Picture *)GmoTim2GetPicture(data, size, index);
  if (pic == NULL) {
    return 0;
  }
  GmoTextureReset(tex);

  levels = pic->mipMapTextures;
  if (levels != 0) {
    fmt = g_gmoTim2FormatMap[(s8)pic->imageType];
    width = (s16)pic->width;
    height = (s16)pic->height;
    image = (GmoImage *)GmoImagePlanTakePalettes(1, arena);
    tex->images = image;
    GmoImageBuild(image, fmt, 0, width, height, 0x10, 1, levels, 1, 1, 3, 1, 0x80, 0, NULL, arena);
    hasMips = levels >= 2;
    src = (u8 *)pic + pic->headerSize;
    for (level = 0; level < levels; level++) {
      GmoTim2ConvertPixels((u8 *)(uintptr_t)GmoTextureGetImageFrame(tex, level, 0), src, width,
                           height, (s8)pic->imageType);
      if (hasMips) {
        width = (width + 1) / 2;
        height = (height + 1) / 2;
        src += pic->mipImageSize[level];
      }
    }
  }

  clutType = pic->clutType & 7;
  if (clutType != 0) {
    fmt = g_gmoTim2FormatMap[clutType];
    image = (GmoImage *)GmoImagePlanTakePalettesThunk(1, arena);
    tex->palettes = image;
    GmoImageBuild(image, fmt, 0, (s16)pic->clutColors, 1, 0x10, 1, 1, 1, 2, 3, 1, 0x10, 0, NULL,
                  arena);
    src = (u8 *)pic + pic->headerSize + pic->imageSize;
    clut = (u32 *)(uintptr_t)GmoTextureGetPaletteFrame(tex, 0, 0);
    GmoTim2ConvertPixels((u8 *)clut, src, (s16)pic->clutColors, 1, clutType);
    if ((s8)pic->clutType >= 0 && (s16)pic->clutColors == 0x100) {
      if (fmt == 1) {
        /* 16-bit entries: 8 blocks of 32 entries (0x40 bytes), swap words 4..7 with 8..11 */
        for (i = 8; i != 0; i--) {
          for (j = 0; j != 4; j++) {
            lo = clut[4 + j];
            hi = clut[8 + j];
            clut[8 + j] = lo;
            clut[4 + j] = hi;
          }
          clut += 0x10;
        }
      } else if (fmt == 3) {
        /* 32-bit entries: 8 blocks of 32 entries (0x80 bytes), swap words 8..15 with 16..23 */
        for (i = 8; i != 0; i--) {
          for (j = 0; j != 8; j++) {
            lo = clut[8 + j];
            hi = clut[16 + j];
            clut[16 + j] = lo;
            clut[8 + j] = hi;
          }
          clut += 0x20;
        }
      }
    }
  }
  sceKernelDcacheWritebackAll();
  return 1;
}

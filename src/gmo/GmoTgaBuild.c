// bdc 0x08a27888 GmoTgaBuild
#include "bdc.h"

/* Build pass of `GmoTextureLoadTga`: reads the header (`GmoTgaReadHeader`), builds the palette
   image from a colour map (converted to RGBA), creates the pixel image (format
   `GmoTgaBppToFormat`, `GmoImageBuild`) and fills it row by row, bottom-up unless descriptor bit
   5 is set, from raw or RLE data (image type bit 8, `GmoTgaConvertPixels`); flushes the data
   cache. Returns 1, or 0 on a bad file. `index` is unused. */

s32 GmoTgaBuild(void *img, void *data, u32 size, s32 index, void *arena)
{
  GmoTexture *tex = img;
  GmoTgaHeader hdr;
  const u8 *src;
  GmoImage *image;
  u8 *dst;
  u32 fmt;
  s32 width;
  s32 height;
  s32 srcBytes;  /* bytes per TGA pixel */
  s32 dstBytes;  /* bytes per image pixel */
  s32 srcRow;    /* bytes per TGA row */
  s32 pitch;     /* signed destination row step */
  s32 i;

  if (img == NULL || arena == NULL) {
    return 0;
  }
  if (GmoTgaCheckHeader(data, size) == 0) {
    return 0;
  }
  GmoTextureReset(tex);
  src = GmoTgaReadHeader(data, &hdr);

  if (hdr.colorMapType != 0) {
    /* palette image: cmapLength x 1, format 3 (8888) */
    s32 count = hdr.cmapLength;
    GmoImage *pal;
    u32 *clut;

    pal = GmoImagePlanTakePalettesThunk(1, arena);
    tex->palettes = pal;
    GmoImageBuild(pal, 3, 0, count, 1, 0x10, 1, 1, 1, 2, 3, 1, 0x10, 0, NULL, arena);
    clut = (u32 *)(uintptr_t)GmoTextureGetPaletteFrame(tex, 0, 0);
    if (hdr.cmapDepth == 24) {
      for (i = 0; i < count; i++) {
        *clut++ = (u32)src[0] << 16 | (u32)src[1] << 8 | (u32)src[2] | 0xff000000;
        src += 3;
      }
    } else {
      for (i = 0; i < count; i++) {
        *clut++ = (u32)src[3] << 24 | (u32)src[0] << 16 | (u32)src[1] << 8 | (u32)src[2];
        src += 4;
      }
    }
  }

  fmt = GmoTgaBppToFormat(hdr.pixelDepth);
  height = hdr.height;
  width = hdr.width;
  image = GmoImagePlanTakePalettes(1, arena);
  tex->images = image;
  GmoImageBuild(image, fmt, 0, width, height, 0x10, 1, 1, 1, 1, 3, 1, 0x80, 0, NULL, arena);
  srcBytes = (hdr.pixelDepth + 7) >> 3;
  dstBytes = GmoImageGetBpp(image) / 8;
  pitch = width * dstBytes + 0xf;
  dst = (u8 *)(uintptr_t)GmoTextureGetImageFrame(tex, 0, 0);
  srcRow = width * srcBytes;
  pitch &= ~0xf;
  if ((hdr.descriptor & 0x20) == 0) {
    /* bottom-up: start at the last row and walk backwards */
    dst += pitch * (height - 1);
    pitch = -pitch;
  }

  if ((hdr.imageType & 8) == 0) {
    /* raw rows */
    for (i = 0; i < height; i++) {
      GmoTgaConvertPixels(dst, src, width, srcBytes, 0);
      src += srcRow;
      dst += pitch;
    }
  } else {
    /* RLE packets: bit 7 = repeat one pixel, low 7 bits = count - 1; packets may span rows */
    s32 x = 0;
    s32 row = 0;

    while (row < height) {
      s8 packet = (s8)*src++;
      s32 count = (packet & 0x7f) + 1;
      s32 repeat = packet < 0;

      while (count > 0) {
        s32 n = width - x;

        if (count < n) {
          n = count;
          count = 0;
        } else {
          count -= n;
        }
        GmoTgaConvertPixels(dst, src, n, srcBytes, repeat);
        dst += n * dstBytes;
        x += n;
        if (!repeat) {
          src += n * srcBytes;
        }
        if (x >= width) {
          dst = dst - dstBytes * x + pitch;
          row++;
          x = 0;
        }
      }
      if (repeat) {
        src += srcBytes;
      }
    }
  }
  sceKernelDcacheWritebackAll();
  return 1;
}

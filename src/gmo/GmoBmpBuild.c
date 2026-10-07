// bdc 0x08a283e4 GmoBmpBuild
#include "bdc.h"

/* Build pass of `GmoTextureLoadBmp`: validates the file, empties the texture, builds the palette
   image from the BMP colour table (only below 16 bpp; 8888 entries via `GmoBmpConvert32`),
   creates the pixel image (`GmoImageBuild`, width x |height|) and converts the bitmap. Compressed
   files: RLE4, RLE8 (buffer cleared first) and 16-bit bitfields (5650, R/B swapped). Uncompressed:
   4-bit (nibbles swapped), 8-bit, 16-bit (5551, R/B swapped, alpha set), 24-bit (to 8888, alpha
   0xff), 32-bit (`GmoBmpConvert32`). Bottom-up bitmaps (positive height) are flipped. Flushes the
   data cache and returns 1; returns 0 when `img` or `arena` is null or the file is not a BMP. */

s32 GmoBmpBuild(void *img, void *data, u32 size, s32 index, void *arena)
{
  GmoTexture *tex = (GmoTexture *)img;
  GmoBmpHeader hdr;
  GmoImage *image;
  u32 fmt;
  s32 colors;
  s32 entries;
  s32 bits;
  s32 width;
  s32 height;
  s32 absHeight;
  s32 rows;
  s32 pitch;
  s32 srcPitch;
  s32 rowBytes;
  s32 clearSize;
  s32 x;
  s32 y;
  s32 i;
  s32 n;
  s32 dx;
  s32 dy;
  u8 value;
  u8 nib;
  u16 px;
  u8 *pixels;
  u8 *rowStart;
  u8 *out;
  u16 *out16;
  const u8 *file = data; /* the BMP file image, read byte-wise */
  const u8 *src;
  const u8 *srcRow;
  const u8 *in;
  const u16 *in16;

  if (img == NULL || arena == NULL) {
    return 0;
  }
  if (GmoBmpCheckHeader((const u16 *)data, size) == 0) {
    return 0;
  }
  GmoTextureReset(tex);
  GmoBmpReadHeader(data, &hdr);
  /* colour table between the 0x36-byte headers and the pixel data, 4 bytes per entry */
  colors = ((s32)hdr.dataOffset - 0x36) / 4;
  if (colors > 0 && (s16)hdr.bitCount < 0x10) {
    entries = 0x100;
    if (colors < 0x11) {
      entries = 0x10;
    }
    image = GmoImagePlanTakePalettesThunk(1, arena);
    tex->palettes = image;
    GmoImageBuild(image, 3, 0, entries, 1, 0x10, 1, 1, 1, 2, 3, 1, 0x10, 0, NULL, arena);
    GmoBmpConvert32((u8 *)(uintptr_t)GmoTextureGetPaletteFrame(tex, 0, 0),
                    file + 0x36, colors, 1, 0xff);
  }
  fmt = GmoBmpBppToFormat((s16)hdr.bitCount, hdr.compression);
  height = hdr.height;
  width = hdr.width;
  image = GmoImagePlanTakePalettes(1, arena);
  tex->images = image;
  absHeight = height;
  if (height < -height) {
    absHeight = -height;
  }
  GmoImageBuild(image, fmt, 0, width, absHeight, 0x10, 1, 1, 1, 1, 3, 1, 0x80, 0, NULL, arena);
  src = file + hdr.dataOffset;
  bits = (s16)hdr.bitCount;
  pixels = (void *)(uintptr_t)GmoTextureGetImageFrame(tex, 0, 0); /* level 0 pixel buffer */

  if (hdr.compression != 0) {
    if (bits == 4) {
      /* RLE4: two pixels per byte, even x in the low nibble of the destination byte */
      pitch = (((width + 1) / 2) + 0xf) & ~0xf;
      rowStart = pixels + pitch * (height - 1);
      clearSize = height * pitch;
      rows = height;
      if (height < 0) {
        rows = -height;
        pitch = -pitch;
        clearSize = -clearSize;
        rowStart = pixels;
      }
      memset(pixels, 0, clearSize);
      if (rows <= 0) {
        goto done;
      }
      out = rowStart;
      x = 0;
      y = 0;
      in = src;
      for (;;) {
        n = in[0];
        if (n == 0) {
          n = in[1];
          if (n < 3) {
            if (n == 1) {
              break; /* end of bitmap */
            }
            if (n == 0) {
              /* end of line */
              rowStart -= pitch;
              y++;
              out = rowStart;
              x = 0;
              in += 2;
            } else {
              /* delta */
              dy = in[3];
              rowStart -= pitch * dy;
              dx = in[2];
              in += 4;
              x += dx;
              y += dy;
              out = rowStart + x / 2;
            }
            if (y >= rows) {
              break;
            }
            continue;
          }
          /* absolute run of n nibbles, padded to a 16-bit boundary */
          in += 2;
          if (width - x < n) {
            n = width - x;
          }
          for (i = 0; i < n; i++) {
            if ((i & 1) == 0) {
              nib = (in[0] >> 4) & 0xf;
            } else {
              nib = in[0] & 0xf;
              in++;
            }
            if ((x & 1) == 0) {
              *out = nib;
            } else {
              *out = (u8)(nib << 4) | *out;
              out++;
            }
            x++;
          }
          in += (n & 1) + (((n + 1) / 2) & 1);
        } else {
          /* encoded run: n pixels alternating the two nibbles of the next byte */
          if (width - x < n) {
            n = width - x;
          }
          value = in[1];
          for (i = 0; i < n; i++) {
            if ((i & 1) == 0) {
              nib = (value >> 4) & 0xf;
            } else {
              nib = value & 0xf;
            }
            if ((x & 1) == 0) {
              *out = nib;
            } else {
              *out = (u8)(nib << 4) | *out;
              out++;
            }
            x++;
          }
          in += 2;
        }
      }
    } else if (bits == 8) {
      /* RLE8 */
      pitch = (width + 0xf) & ~0xf;
      rowStart = pixels + pitch * (height - 1);
      clearSize = height * pitch;
      rows = height;
      if (height < 0) {
        rows = -height;
        pitch = -pitch;
        clearSize = -clearSize;
        rowStart = pixels;
      }
      memset(pixels, 0, clearSize);
      if (rows <= 0) {
        goto done;
      }
      out = rowStart;
      x = 0;
      y = 0;
      in = src;
      for (;;) {
        n = in[0];
        if (n == 0) {
          n = in[1];
          if (n < 3) {
            if (n == 1) {
              break; /* end of bitmap */
            }
            if (n == 0) {
              /* end of line */
              rowStart -= pitch;
              y++;
              out = rowStart;
              x = 0;
              in += 2;
            } else {
              /* delta */
              dy = in[3];
              rowStart -= pitch * dy;
              dx = in[2];
              in += 4;
              x += dx;
              y += dy;
              out = rowStart + x;
            }
            if (y >= rows) {
              break;
            }
            continue;
          }
          /* absolute run of n bytes, padded to a 16-bit boundary (by the clipped count) */
          in += 2;
          if (width - x < n) {
            n = width - x;
          }
          for (i = 0; i < n; i++) {
            *out++ = *in++;
          }
          in += n & 1;
          x += n;
        } else {
          /* encoded run: n copies of the next byte */
          if (width - x < n) {
            n = width - x;
          }
          value = in[1];
          for (i = 0; i < n; i++) {
            *out++ = value;
          }
          in += 2;
          x += n;
        }
      }
    } else if (bits == 0x10) {
      /* 16-bit bitfields: 565 with red and blue swapped */
      pitch = ((width + 7) & ~7) * 2;
      srcPitch = (width + 1) & ~1;
      rowStart = pixels + pitch * (height - 1);
      rows = height;
      if (height < 0) {
        rows = -height;
        pitch = -pitch;
        rowStart = pixels;
      }
      in16 = (const u16 *)src;
      for (y = 0; y < rows; y++) {
        out16 = (u16 *)rowStart;
        for (x = 0; x < width; x++) {
          px = in16[x];
          out16[x] = (px & 0x7e0) | (px >> 11) | (u16)((px & 0x1f) << 11);
        }
        in16 += srcPitch;
        rowStart -= pitch;
      }
    }
    goto done;
  }

  if (bits == 4) {
    /* 4-bit indexed: swap the nibbles of each byte */
    rowBytes = (width + 1) / 2;
    pitch = (rowBytes + 0xf) & ~0xf;
    srcPitch = (rowBytes + 3) & ~3;
    rowStart = pixels + pitch * (height - 1);
    rows = height;
    if (height < 0) {
      rows = -height;
      pitch = -pitch;
      rowStart = pixels;
    }
    srcRow = src;
    for (y = 0; y < rows; y++) {
      for (x = 0; x < rowBytes; x++) {
        value = srcRow[x];
        rowStart[x] = (u8)(value << 4) | (u8)(value >> 4);
      }
      srcRow += srcPitch;
      rowStart -= pitch;
    }
  } else if (bits == 8) {
    /* 8-bit indexed: row copy */
    pitch = (width + 0xf) & ~0xf;
    srcPitch = (width + 3) & ~3;
    rowStart = pixels + pitch * (height - 1);
    rows = height;
    if (height < 0) {
      rows = -height;
      pitch = -pitch;
      rowStart = pixels;
    }
    srcRow = src;
    for (y = 0; y < rows; y++) {
      for (x = 0; x < width; x++) {
        rowStart[x] = srcRow[x];
      }
      srcRow += srcPitch;
      rowStart -= pitch;
    }
  } else if (bits == 0x10) {
    /* 16-bit 555: red and blue swapped, alpha bit set (5551) */
    pitch = ((width + 7) & ~7) * 2;
    srcPitch = (width + 1) & ~1;
    rowStart = pixels + pitch * (height - 1);
    rows = height;
    if (height < 0) {
      rows = -height;
      pitch = -pitch;
      rowStart = pixels;
    }
    in16 = (const u16 *)src;
    for (y = 0; y < rows; y++) {
      out16 = (u16 *)rowStart;
      for (x = 0; x < width; x++) {
        px = in16[x];
        out16[x] = (px & 0x3e0) | ((px >> 10) & 0x1f) | (u16)((px & 0x1f) << 10) | 0x8000;
      }
      in16 += srcPitch;
      rowStart -= pitch;
    }
  } else if (bits == 0x18) {
    /* 24-bit BGR to RGBA 8888, alpha 0xff */
    pitch = (width * 4 + 0xf) & ~0xf;
    srcPitch = (width * 3 + 3) & ~3;
    rowStart = pixels + pitch * (height - 1);
    rows = height;
    if (height < 0) {
      rows = -height;
      pitch = -pitch;
      rowStart = pixels;
    }
    srcRow = src;
    for (y = 0; y < rows; y++) {
      in = srcRow;
      out = rowStart;
      for (x = 0; x < width; x++) {
        /* BGR to R, G, B, A (the asm stores alpha before blue) */
        *out++ = in[2];
        *out++ = in[1];
        *out++ = in[0];
        *out++ = 0xff;
        in += 3;
      }
      srcRow += srcPitch;
      rowStart -= pitch;
    }
  } else if (bits == 0x20) {
    GmoBmpConvert32(pixels, src, width, height, 0);
  }

done:
  sceKernelDcacheWritebackAll();
  return 1;
}

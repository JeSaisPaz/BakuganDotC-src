// bdc 0x08a27614 GmoTgaConvertPixels
#include "bdc.h"

/* Converts `count` TGA pixels to GE order: 1 byte copied, 2 bytes BGR555 → ABGR1555 (red/blue
   swapped, alpha set), 3 bytes BGR → RGBA (alpha 0xff), 4 bytes BGRA → RGBA. With `repeat` set
   (low byte only) the source pointer does not advance (an RLE run). Other depths do nothing. */

/* TGA source pixel bytes (B, G, R[, A]) and a GE RGBA8888 pixel */
typedef struct { u8 b, g, r, a; } TgaPixelBgra;
typedef struct { u8 r, g, b, a; } GePixelRgba;
typedef struct { u8 lo, hi; } TgaPixel16; /* little-endian A1R5G5B5, read bytewise (unaligned) */

void GmoTgaConvertPixels(void *dst, const void *src, s32 count, s32 bytesPerPixel, s32 repeat)
{
  const u8 *s = (const u8 *)src;
  s32 step;
  s32 i;

  step = bytesPerPixel;
  if ((u8)repeat != 0) {
    step = 0;
  }

  if (bytesPerPixel == 2) {
    u16 *d = (u16 *)dst;
    for (i = 0; i < count; i++) {
      const TgaPixel16 *p = (const TgaPixel16 *)s;
      u32 v = ((u32)p->hi << 8) | p->lo;
      *d = (u16)((v & 0x83e0) | ((v >> 10) & 0x1f) | ((v & 0x1f) << 10) | 0x8000);
      s += step;
      d++;
    }
  } else if (bytesPerPixel == 1) {
    u8 *d = (u8 *)dst;
    for (i = 0; i < count; i++) {
      *d = *s;
      s += step;
      d++;
    }
  } else if (bytesPerPixel == 3) {
    GePixelRgba *d = (GePixelRgba *)dst;
    for (i = 0; i < count; i++) {
      const TgaPixelBgra *p = (const TgaPixelBgra *)s;
      u8 b;
      d->r = p->r;
      d->g = p->g;
      b = p->b;
      d->a = 0xff;
      s += step;
      d->b = b;
      d++;
    }
  } else if (bytesPerPixel == 4) {
    GePixelRgba *d = (GePixelRgba *)dst;
    for (i = 0; i < count; i++) {
      const TgaPixelBgra *p = (const TgaPixelBgra *)s;
      d->r = p->r;
      d->g = p->g;
      d->b = p->b;
      d->a = p->a;
      s += step;
      d++;
    }
  }
}

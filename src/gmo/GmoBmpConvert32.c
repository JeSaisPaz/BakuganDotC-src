// bdc 0x08a27fb0 GmoBmpConvert32
#include "bdc.h"

/* Converts a 32-bit BGRA BMP bitmap to RGBA GE rows (16-byte aligned pitch), flipping it vertically
   for bottom-up bitmaps (positive `height`) and OR-ing `alphaOr` into the alpha byte. */

void GmoBmpConvert32(u8 *dst, const u8 *src, s32 width, s32 height, u8 alphaOr)
{
  u8 a;
  u8 *row;
  u8 *out;
  const u8 *in;
  s32 y;
  s32 x;
  u32 pitch;

  pitch = width * 4 + 0xfU & 0xfffffff0;
  row = dst + pitch * (height + -1);
  if (height < 0) {
    height = -height;
    pitch = -pitch;
    row = dst;
  }
  y = 0;
  if (0 < height) {
    do {
      if (0 < width) {
        x = 0;
        in = src;
        out = row;
        do {
          x = x + 1;
          *out = in[2];
          out[1] = in[1];
          out[2] = *in;
          a = in[3];
          in = in + 4;
          out[3] = alphaOr | a;
          out = out + 4;
        } while (width != x);
      }
      src = src + width * 4;
      row = row + -pitch;
      y = y + 1;
    } while (height != y);
  }
}

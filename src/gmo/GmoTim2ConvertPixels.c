// bdc 0x08a26f38 GmoTim2ConvertPixels
#include "bdc.h"

/* Converts TIM2 pixel rows into GE texture rows (each destination row padded to a 16-byte pitch)
   by TIM2 image type: 1 = 16-bit (copied), 2 = 24-bit RGB (alpha 0xff added), 3 = 32-bit (TIM2
   alpha 0..0x80 rescaled to 0..0xff, clamped), 4 = 4-bit indexed ((width + 1) / 2 bytes copied),
   5 = 8-bit indexed (copied). Type 0 or any other value writes nothing. */

void GmoTim2ConvertPixels(u8 *dst, const u8 *src, s32 width, s32 height, s32 type)
{
  u8 *row;
  u8 *out;
  s32 pitch;
  s32 bytes;
  s32 alpha;
  s32 x;
  s32 y;

  row = dst;
  switch (type) {
  case 1:
    pitch = (width * 2 + 0xf) & ~0xf;
    out = dst;
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
        out[0] = src[0];
        out[1] = src[1];
        src += 2;
        out += 2;
      }
      row += pitch;
      out = row;
    }
    break;
  case 2:
    pitch = (width * 4 + 0xf) & ~0xf;
    out = dst;
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
        out[0] = src[0];
        out[1] = src[1];
        out[3] = 0xff;
        out[2] = src[2];
        src += 3;
        out += 4;
      }
      row += pitch;
      out = row;
    }
    break;
  case 3:
    pitch = (width * 4 + 0xf) & ~0xf;
    out = dst;
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
        out[0] = src[0];
        alpha = ((s32)src[3] * 0xff) / 128;
        out[1] = src[1];
        out[2] = src[2];
        out[3] = (alpha < 0x100) ? (u8)alpha : 0xff;
        src += 4;
        out += 4;
      }
      row += pitch;
      out = row;
    }
    break;
  case 4:
    bytes = (width + 1) / 2;
    pitch = (bytes + 0xf) & ~0xf;
    out = dst;
    for (y = 0; y < height; y++) {
      for (x = 0; x < bytes; x++) {
        *out++ = *src++;
      }
      row += pitch;
      out = row;
    }
    break;
  case 5:
    pitch = (width + 0xf) & ~0xf;
    out = dst;
    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
        *out++ = *src++;
      }
      row += pitch;
      out = row;
    }
    break;
  default:
    break;
  }
}

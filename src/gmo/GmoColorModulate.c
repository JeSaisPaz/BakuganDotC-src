// bdc 0x089dd700 GmoColorModulate
#include "bdc.h"

/* Multiplies two RGBA8888 colours channel by channel into `*dst`: each byte is expanded to about
   b/255 (vuc2i + vi2f by 2^31), the two lanes are multiplied, scaled by 255 and packed back with
   vf2iz 23 + vi2uc (floor, clamped to [0, 255]). Returns `dst`. */

u32 *GmoColorModulate(u32 *dst, const u32 *a, const u32 *b)

{
  u32 pa = *a;
  u32 pb = *b;
  u32 result = 0;
  int i;

  for (i = 0; i < 4; i++) {
    u32 ba = (pa >> (i * 8)) & 0xff;
    u32 bb = (pb >> (i * 8)) & 0xff;
    float fa = (float)(int)(ba * 0x01010101u >> 1) / 2147483648.0f;
    float fb = (float)(int)(bb * 0x01010101u >> 1) / 2147483648.0f;
    float prod = fa * fb;

    result |= (u32)VfI2uc(VfF2iz(prod * 255.0f, 23)) << (i * 8);
  }
  *dst = result;
  return dst;
}

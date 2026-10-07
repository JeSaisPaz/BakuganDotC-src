// bdc 0x08a11e48 GmoImageFormatBits
#include "bdc.h"

/* Returns the bits per pixel of image format `fmt` from g_gmoImageFormatBits (16, 16, 16, 32, 4, 8,
   16, 32, 4, 8, ...); only the low 4 bits of `fmt` index the table. */

int GmoImageFormatBits(u32 fmt)
{
  return g_gmoImageFormatBits[fmt & 0xf];
}

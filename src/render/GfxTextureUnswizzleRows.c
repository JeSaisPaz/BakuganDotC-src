// bdc 0x089f6a0c GfxTextureUnswizzleRows
#include "bdc.h"

/* Helper of `GfxTextureBuildDl`: for images at least 256 bytes wide, swaps pairs of 16-byte
   blocks (`format` 1: `+0x10`/`+0x20` in 64-byte groups) or 32-byte blocks (`format` 3:
   `+0x20`/`+0x40` in 128-byte groups) over `size / 32` groups; other formats are untouched. */

void GfxTextureUnswizzleRows(void *data, int format, int size)

{
  u32 *p = (u32 *)data;
  int groups;
  int g;
  int i;
  u32 t;

  if (size < 0x100) {
    return;
  }
  if (format < 2) {
    if (format <= 0) {
      return;
    }
    groups = (size + ((u32)(size >> 5) >> 0x1b)) >> 5;
    for (g = 0; g < groups; g++) {
      for (i = 0; i < 4; i++) {
        t = p[i + 8];
        p[i + 8] = p[i + 4];
        p[i + 4] = t;
      }
      p += 16;
    }
  } else if (format == 3) {
    groups = (size + ((u32)(size >> 5) >> 0x1b)) >> 5;
    for (g = 0; g < groups; g++) {
      for (i = 0; i < 8; i++) {
        t = p[i + 16];
        p[i + 16] = p[i + 8];
        p[i + 8] = t;
      }
      p += 32;
    }
  }
}

// bdc 0x08a25280 GmoPaletteArrayCopy
#include "bdc.h"

/* Build pass for copying `count` palette images: returns NULL when `textures` or `arena` is NULL.
   When `flags & 0x21` carves `count` new 0x30-byte records from the plan
   (`GmoImagePlanTakePalettes`), copies each palette into them (`GmoPaletteCopyOne`) and returns
   the new array; otherwise bumps each record's reference count and returns the original array. */

void *GmoPaletteArrayCopy(void *textures, s32 count, u32 flags, void *arena)
{
  GmoImage *src = (GmoImage *)textures;
  GmoImage *dst;
  GmoImage *copies;
  s32 i;

  if (textures == NULL || arena == NULL) {
    return NULL;
  }
  if ((flags & 0x21) != 0) {
    copies = (GmoImage *)GmoImagePlanTakePalettesThunk(count, arena);
    dst = copies;
    for (i = 0; i < count; i++) {
      GmoPaletteCopyOne(dst, src, flags, arena);
      dst++;
      src++;
    }
    return copies;
  }
  for (i = 0; i < count; i++) {
    src[i].refCount++;
  }
  return textures;
}

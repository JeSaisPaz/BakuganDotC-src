// bdc 0x08a253ec GmoImageArrayCopy
#include "bdc.h"

/* Build pass for copying `count` pixel images: returns NULL when `textures` or `arena` is NULL. When
   `flags & 0x11` carves `count` new 0x30-byte records from the plan (`GmoImagePlanTakePalettes`),
   copies each image into them (`GmoImageCopyOne`) and returns the new array; otherwise bumps each
   image's reference count and returns the original array. */

void *GmoImageArrayCopy(void *textures, s32 count, u32 flags, void *arena)
{
  GmoImage *src = (GmoImage *)textures;
  GmoImage *dst;
  GmoImage *copies;
  s32 i;

  if (textures == NULL || arena == NULL) {
    return NULL;
  }
  if ((flags & 0x11) != 0) {
    copies = (GmoImage *)GmoImagePlanTakePalettes(count, arena);
    dst = copies;
    for (i = 0; i < count; i++) {
      GmoImageCopyOne(dst, src, flags, arena);
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

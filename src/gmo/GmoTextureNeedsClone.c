// bdc 0x08a24788 GmoTextureNeedsClone
#include "bdc.h"

/* Decides whether sharing `count` texture objects (0x40-byte stride) under `flags` needs a deep
   copy: 1 when `flags` has bits other than 2, or when flag 2 is set and an image has its dynamic
   bit (`+2 & 0x10`); 0 means a reference-count bump is enough. */

s32 GmoTextureNeedsClone(const void *images, s32 count, u32 flags)

{
  const GmoTexture *tex = (const GmoTexture *)images;
  s32 i;

  if ((flags & 0xfffd) != 0) {
    return 1;
  }
  if ((flags & 2) == 0) {
    return 0;
  }
  for (i = 0; i < count; i++) {
    if (tex[i].flags & 0x10) {
      return 1;
    }
  }
  return 0;
}

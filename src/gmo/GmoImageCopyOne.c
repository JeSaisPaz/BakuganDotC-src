// bdc 0x08a253e0 GmoImageCopyOne
#include "bdc.h"

/* Thunk to `GmoImageCopy` for pixel images. */

GmoImage *GmoImageCopyOne(GmoImage *dst, const GmoImage *src, u32 flags, void *arena)

{
  return GmoImageCopy(dst,src,flags,arena,0x10,0x80);
}

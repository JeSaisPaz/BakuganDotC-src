// bdc 0x08a24cb0 GmoImageMeasureCopyOne
#include "bdc.h"

/* Thunk to `GmoImageMeasureCopy` for pixel images (kind flag 0x10). */

s32 GmoImageMeasureCopyOne(void *dst, const GmoImage *src, u32 flags, void *arena)

{
  return GmoImageMeasureCopy(dst,src,flags,arena,0x10,0x80);
}


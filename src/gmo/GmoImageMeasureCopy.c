// bdc 0x08a24ab8 GmoImageMeasureCopy
#include "bdc.h"

/* Measure pass for copying one image record: when `src != dst` and `flags & (kindFlag | 1)`,
   reserves the image's storage in the arena by replaying its geometry through `GmoImageMeasure`
   (format, sizes, alignment, levels, frames; 2 levels of residency when bit 31 of `flags` is set).
   Returns 1 (0 for NULL arguments). */

s32 GmoImageMeasureCopy(void *dst, const GmoImage *src, u32 flags, void *arena, u32 kindFlag, u32 gpuFlag)
{
  s32 alignBytes;

  if (src == NULL || arena == NULL) {
    return 0;
  }
  if (src != dst && ((kindFlag | 1) & flags) != 0) {
    alignBytes = GmoImageGetAlignBytes(src);
    GmoImageMeasure(dst, src->format, src->flags16, src->width, src->height, alignBytes,
                    src->heightAlign, src->levelCount, src->frameCount, src->mipmapMode,
                    src->kind29, ((s32)flags < 0) ? 2 : 1, gpuFlag, src->aux2a, 0, arena);
  }
  return 1;
}

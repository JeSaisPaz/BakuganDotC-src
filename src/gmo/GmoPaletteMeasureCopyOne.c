// bdc 0x08a24bcc GmoPaletteMeasureCopyOne
#include "bdc.h"

/* Thunk to `GmoImageMeasureCopy` for palette (CLUT) images (the compiler passed the kind flag
   0x20 through registers). */

s32 GmoPaletteMeasureCopyOne(void *dst, const void *src, u32 flags, void *arena)
{
    return GmoImageMeasureCopy(dst, src, flags, arena, 0x20, 0x10);
}

// bdc 0x08a25274 GmoPaletteCopyOne
#include "bdc.h"

/* Thunk to `GmoImageCopy` for palette images. */

void *GmoPaletteCopyOne(void *dst, const void *src, u32 flags, void *arena)
{
    return GmoImageCopy(dst, src, flags, arena, 0x20, 0x10);
}

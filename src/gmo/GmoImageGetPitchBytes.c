// bdc 0x08a25ea8 GmoImageGetPitchBytes
#include "bdc.h"

/* Returns the row size in bytes of a `GmoImage` for `width` pixels: `width` rounded up to the
   image's width alignment (`widthAlign`, `+0x18`), times its bits per pixel (`bpp`, `+0x17`),
   divided by 8 (signed division, rounding toward zero). 0 for a NULL image. */
s32 GmoImageGetPitchBytes(const GmoImage *img, s32 width)
{
    uint alignMask;
    s32 bits;

    if (img == NULL) {
        return 0;
    }
    alignMask = img->widthAlign - 1;
    bits = (s32)((width + alignMask) & ~alignMask) * (s32)img->bpp;
    return bits / 8;
}

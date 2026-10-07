// bdc 0x08a11d30 GmoTextureComputeScale
#include "bdc.h"

/* Computes the sampling flags of a texture record (`flags`): bit 0/1 when the image list has
   several levels/frames, bit 2/3 the same for the palette list, bits 5|6 for a non-power-of-two
   image (then also storing the UV scale factors `width/pow2` and `height/pow2` in
   `uvTransform[2]`/`uvTransform[3]`), and bit 4 when any of the above multi bits (1|3) is set or
   any animation track has frames. */

/* Smallest power of two >= n, as the MIPS `1 << (32 - clz(n - 1))` (shift amount taken mod 32,
   so n == 0 and n == 1 both give 1). */
static s32 GmoPow2Ceil(u32 n)
{
    u32 v = n - 1;
    s32 lz = (v != 0) ? __builtin_clz(v) : 32;

    return (s32)(1u << ((32 - lz) & 0x1f));
}

void GmoTextureComputeScale(void *tex)
{
    GmoTexture *t = (GmoTexture *)tex;
    GmoImage *img = t->images;
    GmoImage *pal;
    u16 flags = 0;

    if (img != NULL) {
        u32 w = img->width;
        u32 h = img->height;
        s32 pw = GmoPow2Ceil(w);
        s32 ph = GmoPow2Ceil(h);

        flags = (img->levelCount >= 2) ? 1 : 0;
        if (img->frameCount >= 2) {
            flags |= 2;
        }
        if ((u32)pw != w || (u32)ph != h) {
            flags |= 0x60;
            t->uvTransform[2] = (float)(s32)w / (float)pw;
            t->uvTransform[3] = (float)(s32)h / (float)ph;
        }
    }

    pal = t->palettes;
    if (pal != NULL) {
        if (pal->levelCount >= 2) {
            flags |= 4;
        }
        if (pal->frameCount >= 2) {
            flags |= 8;
        }
    }

    if ((flags & 0xa) != 0) {
        flags |= 0x10;
    } else {
        GmoTexTrack *tracks = (GmoTexTrack *)t->tracks;
        s32 n = t->trackCount;
        s32 i;

        for (i = 0; i < n; i++) {
            if (tracks[i].frameCount != 0) {
                flags |= 0x10;
                break;
            }
        }
    }
    t->flags = flags;
}

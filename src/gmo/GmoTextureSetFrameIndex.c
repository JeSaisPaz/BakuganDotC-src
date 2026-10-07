// bdc 0x08a10b08 GmoTextureSetFrameIndex
#include "bdc.h"

/* Selects frame `index` of a `GmoTexture`: stores `index` in `frameIndex` and, from the 32-bit mask
   (`frameMask`) of the first image of its image list, derives the bit position and field value
   of that frame and passes them to `GmoTextureSetFrames` (`(tex, pos, value, 0)`; `(tex, 0, 0xff,
   0)` when the mask is 0). Does nothing for NULL or a texture without images. */
void GmoTextureSetFrameIndex(void *tex_, u8 index)
{
    GmoTexture *tex = tex_;
    u32 mask;
    u32 g;
    u32 acc;
    u32 v;
    u32 pos;
    int i;

    if (tex == NULL) {
        return;
    }
    tex->frameIndex = index;
    if (tex->images == NULL) {
        return;
    }
    mask = tex->images->frameMask;
    if (mask == 0) {
        GmoTextureSetFrames(tex, 0, 0xff, 0);
        return;
    }
    g = mask ^ (mask >> 1);
    acc = 1;
    if (index != 0) {
        for (i = 0; i != index; i++) {
            acc += g ^ (g - acc);
        }
    }
    v = g ^ (g - acc);
    pos = (v != 0) ? (u32)__builtin_ctz(v) : 32;
    GmoTextureSetFrames(tex, (u8)pos, (u8)(v >> (pos & 0x1f)), 0);
}

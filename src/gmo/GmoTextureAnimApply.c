// bdc 0x08a10bfc GmoTextureAnimApply
#include "bdc.h"

/* Applies one animated channel of a `GmoTexture`: `n = floor(value)` goes to the channel `kind`.
   Kind 1 selects the frame (`GmoTextureSetFrameIndex`, tail call); kinds 3 / 11 set both frame
   shorts (`frameA0/frameB0`, `frameA1/frameB1`) of the active track (`lastTrack`, or `trackCount`
   when it is negative or past it); kinds 0x10..0x13 store `value` divided by the image width /
   height rounded up to a power of two into `uvTransform[0..3]` and set flag 0x20 (skipped without
   an image list); kinds 0x20, 0x22..0x27 store `n` as a byte into the filter / wrap / flag bytes
   `+0x30`, `+0x32..+0x37`. Any other kind does nothing. */

/* clz with clz(0) == 32, as the Allegrex `clz` instruction. */
static inline s32 GmoClz32(u32 v)
{
    return (v != 0) ? __builtin_clz(v) : 32;
}

void GmoTextureAnimApply(float value, void *texp, u32 kind)
{
    GmoTexture *tex = (GmoTexture *)texp;
    s32 idx = (s8)tex->lastTrack;
    GmoImage *images = tex->images;
    GmoTexTrack *track;
    s32 n;

    if (idx < 0 || (s32)tex->trackCount < idx) {
        idx = tex->trackCount;
    }
    n = (s32)__builtin_floorf(value); /* floor.w.s, an instruction */
    track = (GmoTexTrack *)tex->tracks + idx;

    switch (kind) {
    case 1:
        GmoTextureSetFrameIndex(tex, (u8)n);
        return;
    case 3:
        if (track != NULL) {
            track->frameA0 = (s16)n;
            track->frameB0 = (s16)n;
        }
        return;
    case 11:
        if (track != NULL) {
            track->frameA1 = (s16)n;
            track->frameB1 = (s16)n;
        }
        return;
    case 0x10:
        if (images != NULL) {
            s32 size = 1 << ((-GmoClz32(images->width - 1)) & 31);
            tex->flags |= 0x20;
            tex->uvTransform[0] = value / (float)size;
        }
        return;
    case 0x11:
        if (images != NULL) {
            s32 size = 1 << ((-GmoClz32(images->height - 1)) & 31);
            tex->flags |= 0x20;
            tex->uvTransform[1] = value / (float)size;
        }
        return;
    case 0x12:
        if (images != NULL) {
            s32 size = 1 << ((-GmoClz32(images->width - 1)) & 31);
            tex->flags |= 0x20;
            tex->uvTransform[2] = value / (float)size;
        }
        return;
    case 0x13:
        if (images != NULL) {
            s32 size = 1 << ((-GmoClz32(images->height - 1)) & 31);
            tex->flags |= 0x20;
            tex->uvTransform[3] = value / (float)size;
        }
        return;
    case 0x20:
        tex->filterMin = (u8)n;
        return;
    case 0x22:
        tex->filterMode = (u8)n;
        return;
    case 0x23:
        tex->filterMag = (u8)n;
        return;
    case 0x24:
        tex->wrapU = (u8)n;
        return;
    case 0x25:
        tex->wrapV = (u8)n;
        return;
    case 0x26:
        tex->flagsA = (u8)n;
        return;
    case 0x27:
        tex->flagsB = (u8)n;
        return;
    default:
        return;
    }
}

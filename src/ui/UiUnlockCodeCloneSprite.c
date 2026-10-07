// bdc 0x08991fdc UiUnlockCodeCloneSprite
#include "bdc.h"

/* Clones a sprite for `UiUnlockCode`: allocates a 0x160-byte `GfxSprite` from
   the low heap, constructs it (`GfxSpriteCtor`), adds it to `layer` (`GfxSpriteLayerAdd`), copies
   `src` into it (`GfxSpriteCopy`) and leaves it hidden. Returns the new sprite. */

GfxSprite *UiUnlockCodeCloneSprite(GfxSprite *src, void *layer)
{
    bool fromLow;
    GfxSprite *sprite;
    GfxSprite *dst;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprite = MemAlloc(sizeof(GfxSprite), (char *)0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    dst = (GfxSprite *)0;
    if (sprite != (GfxSprite *)0) {
        GfxSpriteCtor(sprite);
        dst = sprite;
    }
    GfxSpriteLayerAdd(layer, (CoreObject *)dst);
    GfxSpriteCopy(src, dst);
    dst->flags = dst->flags & 0xfffffffe;
    return dst;
}

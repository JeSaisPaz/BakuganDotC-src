// bdc 0x08808c30 UiLoadIconInit
#include "bdc.h"

/* Constructor of the 'now loading' icon object (`UiLoadIcon`, `g_loadIcon`): runs the task base
   constructor `CoreTaskInit`, installs the vtable `g_loadIconVtbl`, allocates a 0x80-byte sprite
   layer from the low heap (`GfxSpriteLayerCtor`; left NULL when the allocation fails) and creates one
   sprite on it from the texture `load_icon0` (`GfxFindTexture`) at (460.0, 254.0), size 12x12
   (`GfxSpriteSetSize`), quad mode 3, flag `0x20` in `flags`, tint (1,1,1) and alpha 0. Starts hidden
   (`visible = 0`) with `spinSpeed` cleared. Returns `this`. */

void *UiLoadIconInit(void *this)
{
    UiLoadIcon *self = (UiLoadIcon *)this;
    GfxSpriteLayer *layer;
    void *texture;
    bool fromLow;
    float pos[4] __attribute__((aligned(16)));

    CoreTaskInit(&self->base);
    self->base.vtable = g_loadIconVtbl;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    layer = (GfxSpriteLayer *)MemAlloc(0x80, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (layer != NULL) {
        GfxSpriteLayerCtor(layer, 0);
    }
    self->layer = layer;

    texture = GfxFindTexture("load_icon0");
    pos[0] = 460.0f;
    pos[1] = 254.0f;
    pos[2] = 0.0f;
    pos[3] = 0.0f;
    self->sprite = GfxSpriteLayerCreateSprite(layer, texture, pos, false);
    self->sprite->alpha = 0.0f;
    self->sprite->tint[0] = 1.0f;
    self->sprite->tint[1] = 1.0f;
    self->sprite->tint[2] = 1.0f;
    GfxSpriteSetSize(self->sprite, 12.0f, 12.0f);
    GfxSpriteSetQuadMode(self->sprite, 3);
    self->sprite->flags |= 0x20;
    self->visible = 0;
    self->spinSpeed = 0.0f;
    return self;
}

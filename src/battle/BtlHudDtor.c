// bdc 0x0883b344 BtlHudDtor
#include "bdc.h"

/* Calls the deleting destructor (vtable entry 1 of a `CoreObject`, flag 3) of `obj`. */
static void DeleteObject(CoreObject *obj)
{
    const VtblEntry *dtor = &((const VtblEntry *)obj->vtable)[1];

    ((void (*)(void *, s32))dtor->fn)((u8 *)obj + dtor->delta, 3);
}

/* Deletes a sprite layer through entry 1 of its vtable (flag 3). */
static void DeleteLayer(GfxSpriteLayer *layer)
{
    const VtblEntry *dtor = &layer->vtbl[1];

    ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
}

/* Deletes `*layer` and frees `*sprites` (under the memory lock), clearing both. */
static void FreeLayout(GfxSpriteLayer **layer, GfxSprite ***sprites)
{
    if (*layer != NULL) {
        DeleteLayer(*layer);
        *layer = NULL;
    }
    if (*sprites != NULL) {
        MemLock();
        MemFree(*sprites, 0, 0);
        MemUnlock();
        *sprites = NULL;
    }
}

/* Destructor of the HUD/talk task `BtlHud` (id 110): resets the vtable to the HUD's, waits for
   the GE (`GfxWaitGeIdle`), deletes the cut-in textures (virtual deleting destructor, flag 3)
   and frees their aligned pixel buffers (`MemFreeAligned`), deletes the six layouts (sprite
   layer + sprite array), the palette blenders (`GfxPaletteBlendDtor`), the `.fab` objects of
   `fabList` (`CoreObjectListDeleteAll`) and the `fabs` table, the two overlay drawables, the
   package (`IoLzsPackageDtor` flag 2), then chains to `CoreTaskDestroy` and frees the task
   itself when `flags & 1`. A NULL `self` does nothing. */
void BtlHudDtor(BtlHud *self, u32 flags)
{
    int i;

    if (self == NULL) {
        return;
    }
    self->base.vtable = g_btlHudVtbl;
    GfxWaitGeIdle();

    if (self->cutInTexA != NULL) {
        DeleteObject(self->cutInTexA);
        self->cutInTexA = NULL;
    }
    MemFreeAligned(self->cutInPixelsA);
    if (self->cutInTexB != NULL) {
        DeleteObject(self->cutInTexB);
        self->cutInTexB = NULL;
    }
    MemFreeAligned(self->cutInPixelsB);
    for (i = 0; i < 2; i++) {
        if (self->buildTex[i] != NULL) {
            DeleteObject(self->buildTex[i]);
            self->buildTex[i] = NULL;
        }
        MemFreeAligned(self->buildPixels[i]);
    }

    FreeLayout(&self->layer, &self->sprites);
    FreeLayout(&self->resultLayer, &self->resultSprites);
    FreeLayout(&self->msgLayer, &self->msgSprites);
    FreeLayout(&self->ratingLayer, &self->ratingSprites);
    FreeLayout(&self->arenaLayer, &self->arenaSprites);

    if (self->extraBlend != NULL) {
        GfxPaletteBlendDtor(self->extraBlend, 3);
        self->extraBlend = NULL;
    }
    for (i = 0; i < 3; i++) {
        if (self->markerBlend[i] != NULL) {
            GfxPaletteBlendDtor(self->markerBlend[i], 3);
            self->markerBlend[i] = NULL;
        }
    }
    for (i = 0; i < 4; i++) {
        if (self->arrowBlend[i] != NULL) {
            GfxPaletteBlendDtor(self->arrowBlend[i], 3);
            self->arrowBlend[i] = NULL;
        }
    }

    CoreObjectListDeleteAll(&self->fabList);
    if (self->fabs != NULL) {
        MemLock();
        MemFree(self->fabs, 0, 0);
        MemUnlock();
        self->fabs = NULL;
    }
    self->fabs = NULL;
    for (i = 0; i < 2; i++) {
        if (self->overlayObj[i] != NULL) {
            DeleteLayer((GfxSpriteLayer *)self->overlayObj[i]);
            self->overlayObj[i] = NULL;
        }
    }

    IoLzsPackageDtor((IoLzsPackage *)self->package, 2);
    CoreTaskDestroy(&self->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, 0, 0);
        MemUnlock();
    }
}

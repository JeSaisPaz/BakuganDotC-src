// bdc 0x089ee2d4 UiSpriteMngTaskDtor
#include "bdc.h"

/* Destructor of the 2D sprite manager task (id 10090, vtable `g_uiSpriteMngVtable`, see
   `UiSpriteMngEnsureTask`): frees its sprite slot buffer (`sprites`) and deletes its sprite
   layer (`layer`) through the layer's vtable, then `CoreTaskDestroy`; frees itself when
   `flags & 1`. */

void UiSpriteMngTaskDtor(UiSpriteMng *self, u32 flags)

{
  GfxSprite **ptr;
  GfxSpriteLayer *layer;

  if (self != (UiSpriteMng *)0x0) {
    ptr = self->sprites;
    (self->base).vtable = g_uiSpriteMngVtable;
    if (ptr != (GfxSprite **)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      self->sprites = (GfxSprite **)0x0;
    }
    layer = (GfxSpriteLayer *)self->layer;
    if (layer != (GfxSpriteLayer *)0x0) {
      const VtblEntry *dtor = &layer->vtbl[1];
      ((void (*)(void *, int))dtor->fn)((u8 *)layer + dtor->delta,3);
      self->layer = (void *)0x0;
    }
    CoreTaskDestroy(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
}

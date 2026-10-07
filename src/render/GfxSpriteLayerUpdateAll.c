// bdc 0x089f508c GfxSpriteLayerUpdateAll
#include "bdc.h"

/* Calls virtual slot 2 (the per-frame update; `GfxEffectUpdate` for effects, an empty stub
   `0x08a324b0` for plain sprites) on every object in the layer's draw list (`+0x1c`), fetching
   `next` before each call so the object may unlink itself. */

void GfxSpriteLayerUpdateAll(GfxSpriteLayer *self)
{
  GfxSprite *cur = self->head;

  while (cur != (GfxSprite *)0x0) {
    const VtblEntry *update = &((const VtblEntry *)cur->vtable)[2];
    GfxSprite *next = cur->next;

    ((void (*)(void *))update->fn)((u8 *)cur + update->delta);
    cur = next;
  }
}

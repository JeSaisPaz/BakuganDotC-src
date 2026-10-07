// bdc 0x08808d7c UiLoadIconDtor
#include "bdc.h"

/* Destructor of the 'now loading' icon task (vtable `0x08af145c` slot 1): restores the vtable,
   deletes the sprite layer `+0x10` through its virtual destructor (flags 3), clears `g_loadIcon`,
   runs `CoreTaskDestroy` and frees the object when `flags & 1`. */

void UiLoadIconDtor(CoreTask *self, u32 flags)

{
  UiLoadIcon *icon = (UiLoadIcon *)self;
  GfxSpriteLayer *layer;
  const VtblEntry *dtor;

  if (self != (CoreTask *)0x0) {
    layer = icon->layer;
    self->vtable = g_loadIconVtbl;
    if (layer != (GfxSpriteLayer *)0x0) {
      dtor = &layer->vtbl[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
      icon->layer = (GfxSpriteLayer *)0x0;
    }
    g_loadIcon = (void *)0x0;
    CoreTaskDestroy(self, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
}

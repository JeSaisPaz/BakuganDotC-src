// bdc 0x089ee008 GfxScreenFaderDtor
#include "bdc.h"

/* Destructor of the screen fader (vtable `g_gfxScreenFaderVtbl` slot 1): deletes its overlay
   rect, chains to `GfxFaderBaseDtor` and frees it when `flags & 1`. */

void GfxScreenFaderDtor(GfxScreenFader *self, u32 flags)

{
  GfxRect *rect;

  if (self != (GfxScreenFader *)0x0) {
    rect = (GfxRect *)self->rect;
    (self->base).vtbl = g_gfxScreenFaderVtbl;
    if (rect != (GfxRect *)0x0) {
      const VtblEntry *dtor = &rect->vtbl[1];
      ((void (*)(void *, int))dtor->fn)((u8 *)rect + dtor->delta,3);
      self->rect = (void *)0x0;
    }
    GfxFaderBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
}

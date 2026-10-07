// bdc 0x088178a0 UiTextPrinterDtor
#include "bdc.h"

/* Destructor of the text printer: restores the printer vtable, frees the font texture table
   `+0x80`, runs the sprite-group base destructor (`GfxSpriteLayerDtor`) and frees the object when
   `flags & 1`. */

void UiTextPrinterDtor(UiTextPrinter *self, u32 flags)

{
  void **ptr;
  
  if (self != (UiTextPrinter *)0x0) {
    ptr = self->fontTextures;
    (self->layer).vtbl = &g_uiTextPrinterVtbl;
    if (ptr != (void **)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      self->fontTextures = (void **)0x0;
    }
    GfxSpriteLayerDtor(&self->layer,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


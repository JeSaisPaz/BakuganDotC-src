// bdc 0x089290f0 UiHologramViewDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the hologram detail view screen (task id 392): stores its id in the
   last-screen word `0x08ac0e78` and frees its models/sprites. Then `UiScreenDtor``(screen, 0)`;
   frees the object when `flags & 1`. */

void UiHologramViewDtor(UiHologramView *self, u32 flags)

{
  UiTextPrinter *printer;
  const VtblEntry *vt;

  if (self != (UiHologramView *)0x0) {
    (self->base).base.vtable = g_uiHologramViewVtable;
    GfxWaitGeIdle();
    printer = self->printer;
    if (printer != (UiTextPrinter *)0x0) {
      vt = (printer->layer).vtbl;
      ((void (*)(void *, int))vt[1].fn)((char *)printer + vt[1].delta,3);
      self->printer = (UiTextPrinter *)0x0;
    }
    g_lastScreenTaskId = (self->base).base.id;
    UiScreenDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

// bdc 0x08969568 UiCardEquipDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiCardEquip screen (task id 303): reinstalls vtable
   `g_uiCardEquipVtbl`, waits for the GE, deletes its owned sprite/model objects through their virtual
   destructors, stores its task id in `g_lastScreenTaskId` (last closed screen); then
   `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiCardEquipDtor(UiCardEquip *self, u32 flags)

{
  s32 i;

  if (self != (UiCardEquip *)0x0) {
    (self->base).base.vtable = g_uiCardEquipVtbl;
    GfxWaitGeIdle();
    for (i = 0; i < 2; i++) {
      UiTextPrinter *printer = (&self->namePrinter)[i];

      if (printer != (UiTextPrinter *)0x0) {
        const VtblEntry *dtor = &printer->layer.vtbl[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)printer + dtor->delta, 3);
        (&self->namePrinter)[i] = (UiTextPrinter *)0x0;
      }
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

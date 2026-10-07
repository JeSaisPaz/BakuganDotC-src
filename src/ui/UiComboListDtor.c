// bdc 0x089b2f54 UiComboListDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the combo list screen (task id 3002): waits for the GE, restores
   the saved frame-skip and destroys the pack node at `+0x6c`. Then `UiScreenDtor``(screen, 0)`;
   frees the object when `flags & 1`. */

void UiComboListDtor(UiComboList *self, u32 flags)

{
  if (self != (UiComboList *)0x0) {
    (self->base).base.vtable = g_uiComboListVtbl;
    GfxWaitGeIdle();
    g_gfxDisplay->frameSkip = self->savedFrameSkip;
    if (self->pack != (CoreNode *)0x0) {
      const VtblEntry *entry = &((const VtblEntry *)self->pack->vtable)[1];
      ((void (*)(void *, int))entry->fn)((char *)self->pack + entry->delta, 3);
      self->pack = (CoreNode *)0x0;
    }
    UiScreenDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


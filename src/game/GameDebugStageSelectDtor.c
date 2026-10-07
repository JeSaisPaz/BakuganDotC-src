// bdc 0x088cd26c GameDebugStageSelectDtor
#include "bdc.h"

/* Destructor of the developer stage-select menu (id 501): resets the vtable, deletes its three text
   printers through their virtual deleter (slot 1, flag 3), turns D-pad stick emulation off again, chains
   to `CoreTaskDestroy` and frees `self` from the heap when bit 0 of `flags` is set. Does nothing for a
   NULL `self`. */

void GameDebugStageSelectDtor(GameDebugStageSelect *self, u32 flags)
{
  int i;

  if (self == NULL) {
    return;
  }
  self->base.vtable = &g_gameDebugStageSelectVtbl;
  GfxWaitGeIdle();
  for (i = 0; i < 3; i++) {
    UiTextPrinter *printer = self->printers[i];

    if (printer != NULL) {
      const VtblEntry *entry = &printer->layer.vtbl[1];

      ((void (*)(void *, int))entry->fn)((u8 *)printer + entry->delta, 3);
      self->printers[i] = NULL;
    }
  }
  self->pad->dpadEmulatesStick = 0;
  CoreTaskDestroy(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}

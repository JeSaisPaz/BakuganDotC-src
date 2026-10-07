// bdc 0x0896fdb8 UiOptionDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiOption screen (task id 304): reinstalls vtable `g_uiOptionVtbl`,
   waits for the GE, runs `UiOptionDestroyHelpPrinter`, stores its task id in
   `g_lastScreenTaskId` (last closed screen), sets the shared-background hand-over flag
   `g_uiKeepSharedBg`; then `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiOptionDtor(UiOption *self, u32 flags)

{
  if (self != (UiOption *)0x0) {
    (self->base).base.vtable = g_uiOptionVtbl;
    GfxWaitGeIdle();
    g_uiKeepSharedBg = 1;
    UiOptionDestroyHelpPrinter(self);
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


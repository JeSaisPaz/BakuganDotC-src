// bdc 0x089afd34 UiBattleModeSelectDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiBattleModeSelect screen (task id 350): reinstalls vtable
   `g_uiBattleModeSelectVtbl`, waits for the GE, stores its task id in `g_lastScreenTaskId` (last closed
   screen); then `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiBattleModeSelectDtor(UiBattleModeSelect *self, u32 flags)

{
  if (self != (UiBattleModeSelect *)0x0) {
    (self->base).base.vtable = g_uiBattleModeSelectVtbl;
    GfxWaitGeIdle();
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


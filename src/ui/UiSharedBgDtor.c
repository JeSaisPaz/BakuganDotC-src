// bdc 0x0890e0c8 UiSharedBgDtor
#include "bdc.h"

/* Destructor of the shared-background task 320 (vtable `0x08af4814` (`g_uiSharedBgVtable`) slot 1): reinstalls its vtable,
   waits for the GE, runs `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiSharedBgDtor(UiScreen *screen, u32 flags)

{
  if (screen != (UiScreen *)0x0) {
    (screen->base).vtable = &g_uiSharedBgVtable;
    GfxWaitGeIdle();
    UiScreenDtor(screen,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


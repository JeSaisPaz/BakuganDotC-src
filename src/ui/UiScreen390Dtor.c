// bdc 0x08940368 UiScreen390Dtor
#include "bdc.h"

/* Destructor (vtable slot 1) of `UiScreen390`: waits for the GE
   (`GfxWaitGeIdle`), restores the pad's `stickEmulatesDpad` from `+0x76`, stores its task id in
   `g_lastScreenTaskId`, then `UiScreenDtor``(screen, 0)`; frees the object when
   `flags & 1` (`MemLock`, `MemFree`, `MemUnlock`). */

void UiScreen390Dtor(UiScreen *screen, u32 flags)

{
  UiScreen390 *self = (UiScreen390 *)screen;

  if (self != (UiScreen390 *)0x0) {
    self->base.base.vtable = g_uiScreen390Vtable;
    GfxWaitGeIdle();
    self->base.pad->stickEmulatesDpad = self->savedStickEmulatesDpad;
    g_lastScreenTaskId = self->base.base.id;
    UiScreenDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

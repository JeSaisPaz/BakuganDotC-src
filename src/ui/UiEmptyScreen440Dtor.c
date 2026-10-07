// bdc 0x0890ffa8 UiEmptyScreen440Dtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the empty screen class for task id 440: reinstalls vtable
   `0x08af48f4`, runs `UiScreenDtor``(screen, 0)` and frees the object when `flags & 1`. */

void UiEmptyScreen440Dtor(UiScreen *screen, u32 flags)

{
  if (screen != (UiScreen *)0x0) {
    (screen->base).vtable = g_uiEmptyScreen440Vtbl;
    UiScreenDtor(screen,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}


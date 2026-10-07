// bdc 0x0895226c UiEmptyScreen370Dtor
#include "bdc.h"

/* Destructor of the empty screen class id 370 (vtable `0x08af4e24` slot 1): reinstalls the vtable,
   runs `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiEmptyScreen370Dtor(UiScreen *screen, u32 flags)

{
  if (screen != (UiScreen *)0x0) {
    (screen->base).vtable = g_uiEmptyScreen370Vtbl;
    UiScreenDtor(screen,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

